#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QObject>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include "ModeEnum.h"

class FileSelector : public QObject
{
    Q_OBJECT
public:
    explicit FileSelector(QObject *parent = nullptr);

    inline QString getFileName() const { return this->fileName; };
    inline QString getFilePath() const { return this->filePath; };
    inline QString getOutFileName() const {return this->outFileName;};
    inline QString getOutFilePath() const {return this->outFilePath;};


    void selectFile(QWidget* parent, Mode mode);
    QString convertWindowsPathToWsl(const QString& winPath);
    QString getDirectoryPath(QString filePath);
    QString makeOutFIleName(const QString fileName);
    void clearFileSelection();
    void setFileFromPath(const QString& path, Mode mode);

signals:

private:
    QString fileName;
    QString filePath;
    QString outFileName;
    QString outFilePath;
};

#endif // FILESELECTOR_H
