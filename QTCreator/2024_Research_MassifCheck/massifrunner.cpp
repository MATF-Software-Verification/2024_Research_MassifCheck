#include "massifrunner.h"

MassifRunner::MassifRunner(QObject *parent)
    : QObject{parent}
    , process(new QProcess())
{}

MassifRunner::~MassifRunner(){
    delete process;
}

// A function that display a file dialog in the parent widget, with a starting point at home path that allows only
// executable files to be chosen
void MassifRunner::selectExecutable(QWidget* parent){
    filePath = QFileDialog::getOpenFileName(parent,
                                            tr("Open Binary File"),
                                            QDir::homePath(),
                                            tr("Executable Files (*)"));
    fileName = filePath.split('\/').takeLast();
    //need to add --massif-out-file and other arguments to the profiling
    program = QString::fromStdString("wsl valgrind --tool=massif ./") + fileName;
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

    // Go up 3 levels: from /build/Desktop_Qt... → /QTCreator → /2024_Research_MassifCheck
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

void MassifRunner::runMassifCheck(){
    if ( mode == BINARY){
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

