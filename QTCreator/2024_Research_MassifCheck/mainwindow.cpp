#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , massifRunner(new MassifRunner)
{
    ui->setupUi(this);
    QObject::connect(this,  &MainWindow::modeValudeChanged, massifRunner, &MassifRunner::setMode);
}

MainWindow::~MainWindow()
{
    delete massifRunner;
    delete ui;
}

void MainWindow::on_btLoadFile_clicked()
{
    massifRunner->selectExecutable(this);

    // Just for debugging
    //QMessageBox msgBox;
    // msgBox.setText(massifRunner->getFileName() +
    //                "\n with path \n" + massifRunner->getFilePath() +
    //                "\n " + massifRunner->getProgram() +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getFilePath()) +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getMassifFilesDir()));
    // msgBox.exec();

    this->ui->leBinaryFileName->setText(massifRunner->getFileName());
}


void MainWindow::on_btExecute_clicked()
{
    if (!ui->leBinaryFileName->text().isEmpty()){
        massifRunner->runMassifCheck();
    }
}

