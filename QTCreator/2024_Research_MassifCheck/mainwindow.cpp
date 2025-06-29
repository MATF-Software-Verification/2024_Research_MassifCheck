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
    ui->btMassifOptions->setToolTip("Configure Massif settings");
}

MainWindow::~MainWindow()
{
    delete massifRunner;
    delete ui;
}

void MainWindow::on_btLoadFile_clicked()
{
    massifRunner->selectFile(this);

    // Just for debugging
    //QMessageBox msgBox;
    // msgBox.setText(massifRunner->getFileName() +
    //                "\n with path \n" + massifRunner->getFilePath() +
    //                "\n " + massifRunner->getProgram() +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getFilePath()) +
    //                "\n " + massifRunner->convertWindowsPathToWsl(massifRunner->getMassifFilesDir()));
    // msgBox.exec();

    this->ui->leFileName->setText(massifRunner->getFileName());
}

void MainWindow::on_rbCompile_toggled(bool checked){
    if (checked){
        modeValudeChanged(COMPILE);
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_rbBinary_toggled(bool checked){
    if (checked){
        modeValudeChanged(BINARY);
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_rbOutput_toggled(bool checked){
    if (checked){
        modeValudeChanged(OUTPUT);
        this->ui->leFileName->clear();
    }
};

void MainWindow::on_btExecute_clicked()
{
    if (massifRunner->getFilePath().isEmpty() || ui->leFileName->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No file selected.");
        return;
    }
    massifRunner->runMassifCheck();
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

