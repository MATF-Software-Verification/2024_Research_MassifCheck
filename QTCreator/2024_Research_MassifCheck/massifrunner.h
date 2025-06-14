#ifndef MASSIFRUNNER_H
#define MASSIFRUNNER_H

#include <QObject>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

enum Mode{
    COMPILE = 0,
    BINARY = 1,
    OUTPUT = 2
};

class MassifRunner : public QObject
{
    Q_OBJECT
public:
    explicit MassifRunner(QObject *parent = nullptr);
    ~MassifRunner();

    QProcess *process;

    inline QString getFileName() { return this->fileName; };
    inline QString getFilePath() { return this->filePath; };
    inline QString getProgram() { return this->program; };

    inline QStringList getArgs() {return this->args; };
    inline void addArg(QString arg) { args << arg; };



    void selectExecutable(QWidget* parent);
    QString convertWindowsPathToWsl(const QString& winPath);
    QString getMassifFilesDir();
    void runMassifCheck();

private:
    Mode mode = COMPILE;

    QString fileName;
    QString filePath;
    QString program;


    QStringList args;

public slots:
    void setMode(Mode mode) {
        this->mode = mode;
    };

signals:
};

#endif // MASSIFRUNNER_H
