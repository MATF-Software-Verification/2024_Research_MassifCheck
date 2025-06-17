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
        addArg(QString::fromStdString("valgrind"));
        addArg(QString::fromStdString("--tool=massif"));
        QString outFile = convertWindowsPathToWsl(getMassifFilesDir() + "/massif_output.out");
        addArg(QString::fromStdString("--massif-out-file=") + outFile);
        addArg(convertWindowsPathToWsl(getFilePath()));
        process->start("wsl", getArgs());
        process->waitForFinished();
        QMessageBox msgBox;
        msgBox.setText("Massif finished!");
        msgBox.exec();
    }
}

