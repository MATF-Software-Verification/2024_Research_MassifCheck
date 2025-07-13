#include "massifrunner.h"
#include <string>
#include <QDebug>


MassifRunner::MassifRunner(QObject *parent)
    : QObject{parent}
    , process(new QProcess(this))
    , massifOptions(new MassifOptions(this))
{}

MassifRunner::~MassifRunner() = default;


QString MassifRunner::convertWindowsPathToWsl(const QString& winPath) {
    QFileInfo fileInfo(winPath);
    QString absolutePath = QDir::toNativeSeparators(fileInfo.absoluteFilePath());

    if (!absolutePath.contains(":\\")) {
        return QString();  // Not a Windows-style path
    }

    QString driveLetter = absolutePath.left(1).toLower();
    QString pathWithoutDrive = absolutePath.mid(2); // skip "C:"
    QString wslPath = "/mnt/" + driveLetter + pathWithoutDrive;
    wslPath.replace("\\", "/"); // convert backslashes to slashes

    return wslPath;
}

QString MassifRunner::getMassifFilesDir() {
    // Start from the directory where the executable is
    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();


    QString massifPath = dir.absoluteFilePath("massif_files");

    QDir targetDir(massifPath);
    if (!targetDir.exists()) {
        if (!dir.mkdir("massif_files")) {
            qWarning() << "Failed to create massif_files directory at" << massifPath;
            return QString();
        }
    }

    return massifPath;
}

void MassifRunner::runMassifCheck(FileSelector& fileSelector, Mode mode){
    args.clear();

    if ( mode == COMPILE ){
        addArg(QString::fromStdString("g++"));
        addArg(QString::fromStdString("-g"));
        addArg(convertWindowsPathToWsl(fileSelector.getFilePath()));
        addArg(QString::fromStdString("-o"));
        addArg(convertWindowsPathToWsl(fileSelector.getOutFilePath() + fileSelector.getOutFileName()));
        process->start("wsl", getArgs());
        process->waitForFinished();
        QMessageBox msgBox;
        msgBox.setText("Compile finished!");
        msgBox.exec();
    }
    else if ( mode == BINARY){
        QString massifOut = convertWindowsPathToWsl(getNextMassifOutFilePath());
        QString exePath = convertWindowsPathToWsl(fileSelector.getFilePath());

        // Komanda koja pokreće valgrind u WSL i čeka ENTER da zatvori terminal
        QString command = QString("valgrind --tool=massif " + massifOptions->makeAdditionalArguments() + " --massif-out-file=%1 %2; echo '--- Done ---'; read")
                              .arg(massifOut, exePath);

        // Using cmd.exe to open a new terminal on widows in case the .out is an interactive program
        args.clear();
        args << "/c" << "start" << "wsl.exe" << "-e" << "bash" << "-c" << command;

        bool started = QProcess::startDetached("cmd.exe", args);
        if (!started) {
            QMessageBox::warning(nullptr, "Error", "Failed to launch Valgrind in terminal.");
        }
    }
}

QString MassifRunner::runMassifOutputAnalysis(FileSelector& fileSelector) {
    Parser parser;
    auto [header, snapshots] = parser.parseMassifFile(fileSelector.getFilePath());

    if (snapshots.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "No snapshots found in the file.");
        return "";
    }

    MassifAnalyzer analyzer;
    QString text = analyzer.detectMemoryLeaks(snapshots);

    QMessageBox::information(nullptr, "Analysis", "Memory analysis completed. Press OK for results.");
    return text;

}

QString MassifRunner::MassifGraphUsingMsPrint(const FileSelector& massifSelector) {
    QString massifFilePath = convertWindowsPathToWsl(massifSelector.getFilePath());

    QStringList args;
    args << "ms_print" << massifFilePath;

    QProcess process;
    process.start("wsl", args);

    bool finished = process.waitForFinished(10000);
    if (!finished) {
        qWarning() << "ms_print process timed out";
        return "Error: ms_print process timed out.";
    }

    QByteArray output = process.readAllStandardOutput();
    QByteArray errorOutput = process.readAllStandardError();

    if (!errorOutput.isEmpty()) {
        qWarning() << "ms_print error:" << errorOutput;
        return "Error running ms_print:\n" + QString(errorOutput);
    }

    return QString::fromUtf8(output);
}



QString MassifRunner::getNextMassifOutFilePath() {
    QString massifDir = getMassifFilesDir();
    QDir dir(massifDir);
    QStringList files = dir.entryList(QStringList() << "massif.out.*", QDir::Files);

    int maxIndex = -1;
    QRegularExpression regex(R"(massif\.out\.(\d+))");

    for (const QString& file : files) {
        QRegularExpressionMatch match = regex.match(file);
        if (match.hasMatch()) {
            int index = match.captured(1).toInt();
            if (index > maxIndex) {
                maxIndex = index;
            }
        }
    }

    int nextIndex = maxIndex + 1;
    QString fileName = QString("massif.out.%1").arg(nextIndex);
    return getMassifFilesDir() + "/" + fileName;
}


void MassifRunner::setMassifOptions(MassifOptions *options)
{
    massifOptions->includeHeapProfiling = options->includeHeapProfiling;
    massifOptions->includeStackProfiling = options->includeStackProfiling;
    massifOptions->timeUnit = options->timeUnit;
    massifOptions->maxSnapshots = options->maxSnapshots;
}
