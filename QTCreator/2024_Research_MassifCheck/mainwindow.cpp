#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include <QDir>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , massifRunner(new MassifRunner)
    , fileSelector(new FileSelector)
    , massifSelector(new FileSelector)
{
    ui->setupUi(this);
    ui->btMassifOptions->setToolTip("Configure Massif settings");
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


void MainWindow::on_btMassifOptions_clicked()
{
    MassifOptionsWindow massifOptionsWindow;

    QObject::connect(&massifOptionsWindow, &MassifOptionsWindow::optionsChanged, this, &MainWindow::setMassifOptions);

    massifOptionsWindow.exec();
}

void MainWindow::setMassifOptions(MassifOptions *options)
{
    massifRunner->setMassifOptions(options);
}


void MainWindow::on_btLoadFile_2_clicked()
{
    massifSelector->selectFile(this, OUTPUT);
    this->ui->leFileName_2->setText(massifSelector->getFileName());
}


void MainWindow::on_btShowResult_clicked()
{
    if (massifSelector->getFilePath().isEmpty() || ui->leFileName_2->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No file selected.");
        return;
    }
    massifRunner->runMassifCheck(*massifSelector, OUTPUT);
}

