#include "massifrunner.h"
#include <string>
#include <QDebug>
#include <QOperatingSystemVersion>


MassifRunner::MassifRunner(QObject *parent)
    : QObject{parent}
    , process(new QProcess(this))
    , massifOptions(new MassifOptions(this))
    , massifAnalyzerThresholds(new MassifAnalyzerThresholds(this))
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

bool MassifRunner::runMassifCheck(FileSelector& fileSelector, Mode mode){
    args.clear();

    if ( mode == COMPILE ){
        if (fileSelector.getFileName().endsWith(".cpp")){
            addArg(QString::fromStdString("g++"));
        }
        else {
            addArg(QString::fromStdString("gcc"));
        }
        addArg(QString::fromStdString("-g"));
        addArg(isWindowsHost() ? convertWindowsPathToWsl(fileSelector.getFilePath()) : fileSelector.getFilePath());
        addArg(QString::fromStdString("-o"));
        addArg(isWindowsHost() ? convertWindowsPathToWsl(fileSelector.getOutFilePath() + fileSelector.getOutFileName())
                               : fileSelector.getOutFilePath() + fileSelector.getOutFileName());
        if (isWindowsHost()) {
            process->start("wsl", getArgs());
        } else {
            process->start(getArgs().first(), getArgs().mid(1)); // directly on Linux
        }
        process->waitForFinished();
        QMessageBox msgBox;
        msgBox.setText("Compile finished!");
        msgBox.exec();

        fileSelector.setFileName(fileSelector.getOutFileName());
        fileSelector.setFilePath(fileSelector.getOutFilePath() + fileSelector.getOutFileName());
        mode = BINARY;
    }
    if ( mode == BINARY){
        QString massifOut = isWindowsHost() ? convertWindowsPathToWsl(getNextMassifOutFilePath()) : getNextMassifOutFilePath();
        QString exePath = isWindowsHost() ? convertWindowsPathToWsl(fileSelector.getFilePath()) : fileSelector.getFilePath();

        if (isWindowsHost()) {
            QString command = QString("valgrind --tool=massif %1 --massif-out-file=%2 %3; echo '--- Done ---'; read")
            .arg(massifOptions->makeAdditionalArguments(), massifOut, exePath);

            args.clear();
            args << "/c" << "start" << "wsl.exe" << "-e" << "bash" << "-c" << command;

            if (!QProcess::startDetached("cmd.exe", args)) {
                QMessageBox::warning(nullptr, "Error", "Failed to launch Valgrind in terminal.");
                return false;
            }
        } else {
            QStringList valgrindArgs = {"--tool=massif"};
            valgrindArgs += massifOptions->makeAdditionalArguments().split(' ', Qt::SkipEmptyParts);
            valgrindArgs << "--massif-out-file=" + massifOut << exePath;

            // Pokreni u novom terminalu
            if (!QProcess::startDetached("x-terminal-emulator", QStringList{"-e", "valgrind"} + valgrindArgs)) {
                QMessageBox::warning(nullptr, "Error", "Failed to launch Valgrind in terminal.");
                return false;
            }
        }

        // // Komanda koja pokreće valgrind u WSL i čeka ENTER da zatvori terminal
        // QString command = QString("valgrind --tool=massif " + massifOptions->makeAdditionalArguments() + " --massif-out-file=%1 %2; echo '--- Done ---'; read")
        //                       .arg(massifOut, exePath);

        // // Using cmd.exe to open a new terminal on widows in case the .out is an interactive program
        // args.clear();
        // args << "/c" << "start" << "wsl.exe" << "-e" << "bash" << "-c" << command;

        // bool started = QProcess::startDetached("cmd.exe", args);
        // if (!started) {
        //     QMessageBox::warning(nullptr, "Error", "Failed to launch Valgrind in terminal.");
        //     return false;
        // }
    }

    return true;
}

QString MassifRunner::runMassifOutputAnalysis(FileSelector& fileSelector) {
    Parser parser;
    auto [header, snapshots] = parser.parseMassifFile(fileSelector.getFilePath());

    if (snapshots.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "No snapshots found in the file.");
        return "";
    }

    MassifAnalyzer analyzer;
    QString text = analyzer.detectMemoryLeaks(snapshots, this->massifAnalyzerThresholds);
    auto functionSummary = parser.summarizeAllocationsByFunction(snapshots);
    text += "\n" + analyzer.generateFunctionAllocationReport(functionSummary, this->massifAnalyzerThresholds);

    QMessageBox::information(nullptr, "Analysis", "Memory analysis completed. Press OK for results.");
    return text;

}

QString MassifRunner::MassifGraphUsingMsPrint(const FileSelector& massifSelector) {
    QString massifFilePath = isWindowsHost()
                                 ? convertWindowsPathToWsl(massifSelector.getFilePath())
                                 : massifSelector.getFilePath();

    QStringList args;
    args << "ms_print" << massifFilePath;

    QProcess process;

    if (isWindowsHost()) {
        process.start("wsl", args);
    } else {
        process.start(args.first(), args.mid(1));
    }

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

    // QProcess process;
    // process.start("wsl", args);

    // bool finished = process.waitForFinished(10000);
    // if (!finished) {
    //     qWarning() << "ms_print process timed out";
    //     return "Error: ms_print process timed out.";
    // }

    // QByteArray output = process.readAllStandardOutput();
    // QByteArray errorOutput = process.readAllStandardError();

    // if (!errorOutput.isEmpty()) {
    //     qWarning() << "ms_print error:" << errorOutput;
    //     return "Error running ms_print:\n" + QString(errorOutput);
    // }

    // return QString::fromUtf8(output);
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

    delete options;
}

void MassifRunner::setMassifAnalyzerThresholds(MassifAnalyzerThresholds *thresholds){
    this->massifAnalyzerThresholds->setThresholds(thresholds);
    delete(thresholds);
}

bool MassifRunner::isWindowsHost() const {
    return QOperatingSystemVersion::currentType() == QOperatingSystemVersion::Windows;
}
