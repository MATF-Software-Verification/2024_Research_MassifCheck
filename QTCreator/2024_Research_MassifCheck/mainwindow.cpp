#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , massifRunner(new MassifRunner)
    , fileSelector(new FileSelector)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete massifRunner;
    delete fileSelector;
    delete ui;
}

void MainWindow::on_btLoadFile_clicked()
{
    fileSelector->selectFile(this, mode);

    // Just for debugging
    //QMessageBox msgBox;
    // msgBox.setText(massifRunner->getFileName() +
    //                "\n with path \n" + massifRunner->getFilePath() +
    //                "\n " + massifRunner->getProgram() +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getFilePath()) +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getMassifFilesDir()));
    // msgBox.exec();

    this->ui->leFileName->setText(fileSelector->getFileName());
}

void MainWindow::on_rbCompile_toggled(bool checked){
    if (checked){
        mode = COMPILE;
        fileSelector->clearFileSelection();
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_rbBinary_toggled(bool checked){
    if (checked){
        mode = BINARY;
        fileSelector->clearFileSelection();
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_rbOutput_toggled(bool checked){
    if (checked){
        mode = OUTPUT;
        fileSelector->clearFileSelection();
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_btExecute_clicked()
{
    if (fileSelector->getFilePath().isEmpty() || ui->leFileName->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No file selected.");
        return;
    }
    massifRunner->runMassifCheck(*fileSelector, mode);
}

