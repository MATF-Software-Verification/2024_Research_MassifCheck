#include "mainwindow.h"
#include <QMessageBox>
#include "./ui_mainwindow.h"
#include <QDir>
#include <QStyle>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , massifRunner(new MassifRunner(this))
    , fileSelector(new FileSelector(this))
    , massifSelector(new FileSelector(this))
{
    ui->setupUi(this);
    ui->btMassifOptions->setToolTip("Configure Massif settings");
    ui->btMassifAnalyzerParameters->setToolTip("Configure thresholds for report analysis");
    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon));
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon);
    trayIcon->setIcon(icon);
    trayIcon->show();

    this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btLoadFile_clicked()
{
    fileSelector->selectFile(this, mode);

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
    if ( massifRunner->runMassifCheck(*fileSelector, mode) ){
        massifSelector->setFileFromPath(newMassifFilePath, OUTPUT);
        this->ui->leSelectedMassifOutFile->setText(QFileInfo(newMassifFilePath).fileName());
    }

}


void MainWindow::on_btMassifOptions_clicked()
{
    MassifOptionsWindow massifOptionsWindow;
    massifOptionsWindow.setOptionsFields(massifRunner->getOptions());

    QObject::connect(&massifOptionsWindow, &MassifOptionsWindow::optionsChanged, this, &MainWindow::setMassifOptions);

    massifOptionsWindow.exec();
}

void MainWindow::setMassifOptions(MassifOptions *options)
{
    massifRunner->setMassifOptions(options);
}

void MainWindow::setMassifAnalyzerThresholds(MassifAnalyzerThresholds *thresholds){
    massifRunner->setMassifAnalyzerThresholds(thresholds);
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



void MainWindow::on_btMassifAnalyzerParameters_clicked()
{
    MassifAnalyzerThresholdsWindow massifAnalyzerThresholdsWindow;
    massifAnalyzerThresholdsWindow.setThresholdsFields(massifRunner->getThresholds());

    QObject::connect(&massifAnalyzerThresholdsWindow, &MassifAnalyzerThresholdsWindow::thresholdsChanged, this, &MainWindow::setMassifAnalyzerThresholds);

    massifAnalyzerThresholdsWindow.exec();
}

