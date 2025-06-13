#ifndef MASSIFRUNNER_H
#define MASSIFRUNNER_H

#include <QObject>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

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
    void runMassif();

private:
    QString fileName;
    QString filePath;
    QString program;


    QStringList args;

signals:
};

#endif // MASSIFRUNNER_H
