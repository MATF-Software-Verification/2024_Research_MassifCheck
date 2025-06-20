#ifndef MASSIFRUNNER_H
#define MASSIFRUNNER_H

#include <QObject>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include "ModeEnum.h"

class MassifRunner : public QObject
{
    Q_OBJECT
public:
    explicit MassifRunner(QObject *parent = nullptr);
    ~MassifRunner();

    QProcess *process;

    inline QString getFileName() { return this->fileName; };
    inline QString getFilePath() { return this->filePath; };

    inline QStringList getArgs() {return this->args; };
    inline void addArg(QString arg) { args << arg; };



    void selectFile(QWidget* parent);
    QString convertWindowsPathToWsl(const QString& winPath);
    QString getDirectoryPath(QString filePath);
    QString replaceCppWithOut(const QString fileName);
    QString getMassifFilesDir();
    void runMassifCheck();
    QString getNextMassifOutFilePath();
    void clearFileSelection();

private:
    Mode mode = COMPILE;

    QString fileName;
    QString filePath;
    QString outFileName;
    QString outFilePath;


    QStringList args;
    void runMassifOutputAnalysis();

public slots:
    void setMode(Mode mode) {
        this->mode = mode;
        clearFileSelection();
    };

signals:
};

#endif // MASSIFRUNNER_H
