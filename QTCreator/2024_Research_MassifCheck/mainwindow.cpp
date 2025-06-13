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
}

MainWindow::~MainWindow()
{
    delete massifRunner;
    delete ui;
}

void MainWindow::on_btLoadBinary_clicked()
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


void MainWindow::on_btRunAnalysis_clicked()
{
    if (!ui->leBinaryFileName->text().isEmpty()){
        massifRunner->runMassif();
    }
}

