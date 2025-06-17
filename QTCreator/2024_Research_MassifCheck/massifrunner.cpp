#include "massifrunner.h"
#include <string>

MassifRunner::MassifRunner(QObject *parent)
    : QObject{parent}
    , process(new QProcess())
{}

MassifRunner::~MassifRunner(){
    delete process;
}


void MassifRunner::selectFile(QWidget* parent){
    std::string description;
    std::string fileType;
    if ( mode == COMPILE){
        description = "Open Source File";
        fileType = "C++ Files (*.cpp)";
    }
    else if ( mode == BINARY){
        description = "Open Binary File";
        fileType = "Executable Files (*)";
    }

    filePath = QFileDialog::getOpenFileName(parent,
                                            tr(description.c_str()),
                                            QDir::homePath(),
                                            tr(fileType.c_str()));
    fileName = filePath.split('\/').takeLast();
    outFileName = replaceCppWithOut(fileName);
    outFilePath = getDirectoryPath(filePath);
}

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

    // Go into massif_files
    if (dir.cd("massif_files")) {
        return dir.absolutePath();
    } else {
        qWarning() << "massif_files directory not found!";
        return QString();
    }
}

QString MassifRunner::replaceCppWithOut(const QString fileName) {
    if (fileName.endsWith(".cpp", Qt::CaseInsensitive)) {
        return fileName.left(fileName.length() - 4) + ".out";
    }
    return fileName + ".out";
}

QString MassifRunner::getDirectoryPath(QString filePath){
    int lastSlash = filePath.lastIndexOf(fileName);

    if (lastSlash != -1) {
        return filePath.left(lastSlash);
    }
    return QString();
}

void MassifRunner::runMassifCheck(){
    if ( mode == COMPILE ){
        addArg(QString::fromStdString("g++"));
        addArg(QString::fromStdString("-g"));
        addArg(convertWindowsPathToWsl(getFilePath()));
        addArg(QString::fromStdString("-o"));
        addArg(convertWindowsPathToWsl(outFilePath + outFileName));
        process->start("wsl", getArgs());
        process->waitForFinished();
        QMessageBox msgBox;
        msgBox.setText("Compile finished!");
        msgBox.exec();
    }
    else if ( mode == BINARY){
        QString massifOut = convertWindowsPathToWsl(getMassifFilesDir() + "/massif_output.out");
        QString exePath = convertWindowsPathToWsl(getFilePath());

        // Komanda koja pokreće valgrind u WSL i čeka ENTER da zatvori terminal
        QString command = QString("valgrind --tool=massif --massif-out-file=%1 %2; echo '--- Done ---'; read")
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

