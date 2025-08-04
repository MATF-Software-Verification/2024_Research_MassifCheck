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
#include "fileselector.h"
#include "massifoptions.h"
#include "massifanalyzerthresholds.h"
#include "Parser.h"
#include "massifanalyzer.h"


class MassifRunner : public QObject
{
    Q_OBJECT
public:
    explicit MassifRunner(QObject *parent = nullptr);
    ~MassifRunner();

    QProcess *process;

    inline QStringList getArgs() {return this->args; }
    inline void addArg(QString arg) { args << arg; }


    QString convertWindowsPathToWsl(const QString& winPath);
    QString getMassifFilesDir();
    bool runMassifCheck(FileSelector& fileSelector, Mode mode);
    QString getNextMassifOutFilePath();
    void setMassifOptions(MassifOptions* options);
    void setMassifAnalyzerThresholds(MassifAnalyzerThresholds *thresholds);
    QString runMassifOutputAnalysis(FileSelector& fileSelector);
    QString MassifGraphUsingMsPrint(const FileSelector& massifSelector);

    inline MassifAnalyzerThresholds* getThresholds(){return massifAnalyzerThresholds;};
    inline MassifOptions* getOptions(){return massifOptions;}

    bool isWindowsHost() const;

private:
    Mode mode = COMPILE;

    QStringList args;

    MassifOptions* massifOptions;
    MassifAnalyzerThresholds* massifAnalyzerThresholds;

};

#endif // MASSIFRUNNER_H
