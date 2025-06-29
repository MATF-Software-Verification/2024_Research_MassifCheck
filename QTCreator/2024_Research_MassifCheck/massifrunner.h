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
#include "Parser.h"
#include "fileselector.h"
#include "massifanalyzer.h"
#include "massifoptions.h"

class MassifRunner : public QObject
{
    Q_OBJECT
public:
    explicit MassifRunner(QObject *parent = nullptr);
    ~MassifRunner();

    QProcess *process;

    inline QStringList getArgs() {return this->args; };
    inline void addArg(QString arg) { args << arg; };


    QString convertWindowsPathToWsl(const QString& winPath);
    QString getMassifFilesDir();
    void runMassifCheck(FileSelector& fileSelector, Mode mode);
    QString getNextMassifOutFilePath();
    void setMassifOptions(MassifOptions* options);

private:
    Mode mode = COMPILE;

    QStringList args;
    void runMassifOutputAnalysis(FileSelector& fileSelector);

    MassifOptions* massifOptions;

signals:
};

#endif // MASSIFRUNNER_H
