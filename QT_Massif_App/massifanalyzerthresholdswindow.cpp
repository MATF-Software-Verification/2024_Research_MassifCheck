#include "massifanalyzerthresholdswindow.h"
#include "ui_massifanalyzerthresholdswindow.h"

MassifAnalyzerThresholdsWindow::MassifAnalyzerThresholdsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MassifAnalyzerThresholdsWindow)
{
    ui->setupUi(this);
}

void MassifAnalyzerThresholdsWindow::setThresholdsFields(MassifAnalyzerThresholds *thresholds)
{
    ui->sbFragmentationThreshold->setValue(thresholds->fragmentationThreshold * 100);
    ui->sbHighAllocationCount->setValue(thresholds->highAllocationCount);
    ui->sbHighMemoryThreshold->setValue(thresholds->highMemoryThreshold / BYTES_TO_MB);
    ui->sbLargeMemoryThreshold->setValue(thresholds->largeMemoryThreshold / BYTES_TO_MB);
    ui->sbMemoryFreeThreshold->setValue(thresholds->memoryFreeThreshold);
    ui->sbMemoryJumpThreshold->setValue(thresholds->memoryJumpThreshold * 100);
    ui->sbSmallTotalAllocation->setValue(thresholds->smallTotalAllocation / BYTES_TO_MB);
}

MassifAnalyzerThresholdsWindow::~MassifAnalyzerThresholdsWindow()
{
    delete ui;
}

void MassifAnalyzerThresholdsWindow::on_buttonBox_accepted()
{
    auto massifAnalyzerThresholds = new MassifAnalyzerThresholds();
    massifAnalyzerThresholds->fragmentationThreshold = ui->sbFragmentationThreshold->text().toDouble() / 100;
    massifAnalyzerThresholds->highAllocationCount = ui->sbHighAllocationCount->text().toInt();
    massifAnalyzerThresholds->highMemoryThreshold = ui->sbHighMemoryThreshold->text().toLongLong() * BYTES_TO_MB;
    massifAnalyzerThresholds->largeMemoryThreshold = ui->sbLargeMemoryThreshold->text().toLongLong() * BYTES_TO_MB;
    massifAnalyzerThresholds->memoryFreeThreshold = ui->sbMemoryFreeThreshold->text().toLongLong();
    massifAnalyzerThresholds->memoryJumpThreshold = ui->sbMemoryJumpThreshold->text().toDouble() / 100;
    massifAnalyzerThresholds->smallTotalAllocation = ui->sbSmallTotalAllocation->text().toLongLong() * BYTES_TO_MB;

    emit thresholdsChanged(massifAnalyzerThresholds);
}

