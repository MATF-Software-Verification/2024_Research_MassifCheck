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
    delete massifSelector;
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

void MainWindow::on_btExecute_clicked()
{
    if (fileSelector->getFilePath().isEmpty() || ui->leFileName->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No file selected.");
        return;
    }

    QString newMassifFilePath = massifRunner->getNextMassifOutFilePath();
    massifRunner->runMassifCheck(*fileSelector, mode);

    // this if is a temporary solution until automatic compilation is developed (not only for binary)
    // should also refactor the runMassifCheck function so that it returns boolean = true if successful, so that can also be checked in the if
    if(mode == BINARY){
        massifSelector->setFileFromPath(newMassifFilePath, OUTPUT);
        this->ui->leSelectedMassifOutFile->setText(QFileInfo(newMassifFilePath).fileName());
    }

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


void MainWindow::on_btLoadMassifOutFile_clicked()
{
    massifSelector->selectFile(this, OUTPUT);
    this->ui->leSelectedMassifOutFile->setText(massifSelector->getFileName());
}


void MainWindow::on_btShowResult_clicked()
{
    if (massifSelector->getFilePath().isEmpty() || ui->leSelectedMassifOutFile->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No file selected.");
        return;
    }

    QString text = massifRunner->runMassifOutputAnalysis(*massifSelector);
    QString output = massifRunner->MassifGraphUsingMsPrint(*massifSelector);


    ResultDialog dialog(this);
    dialog.setText(text);
    dialog.setGraph(output);
    dialog.exec();
}


