#include "fileselector.h"

FileSelector::FileSelector(QObject *parent)
    : QObject{parent}
{}

void FileSelector::selectFile(QWidget* parent, Mode mode){
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
    else if (mode == OUTPUT) {
        description = "Select Massif Output File";
        fileType = "Massif Output Files (massif.out.*)";
    }

    filePath = QFileDialog::getOpenFileName(parent,
                                            tr(description.c_str()),
                                            QDir::homePath(),
                                            tr(fileType.c_str()));

    fileName = QFileInfo(filePath).fileName();

    if ( mode == COMPILE){
        outFileName = replaceCppWithOut(fileName);
        outFilePath = getDirectoryPath(filePath);
    }

}

QString FileSelector::convertWindowsPathToWsl(const QString& winPath) {
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

QString FileSelector::getDirectoryPath(QString filePath){
    int lastSlash = filePath.lastIndexOf(fileName);

    if (lastSlash != -1) {
        return filePath.left(lastSlash);
    }
    return QString();
}

QString FileSelector::replaceCppWithOut(const QString fileName) {
    if (fileName.endsWith(".cpp", Qt::CaseInsensitive)) {
        return fileName.left(fileName.length() - 4) + ".out";
    }
    return fileName + ".out";
}

void FileSelector::clearFileSelection()
{
    fileName.clear();
    filePath.clear();
    outFileName.clear();
    outFilePath.clear();
}
