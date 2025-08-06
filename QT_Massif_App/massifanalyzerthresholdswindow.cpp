#include "massifanalyzerthresholdswindow.h"
#include "ui_massifanalyzerthresholdswindow.h"

MassifAnalyzerThresholdsWindow::MassifAnalyzerThresholdsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MassifAnalyzerThresholdsWindow)
{
    ui->setupUi(this);
    QPixmap pixmap = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16, 16);
    ui->lbHighMemoryThresholdHelp->setPixmap(pixmap);
    ui->lbHighMemoryThresholdHelp->setToolTip("The minimum size (in MB) of a single allocation that is considered 'large'."
                                              " Used to detect unusually big memory blocks.");

    ui->lbHighAllocationCountHelp->setPixmap(pixmap);
    ui->lbHighAllocationCountHelp->setToolTip("The number of individual allocations above which a function or test is flagged as 'frequently allocating'."
                                              " Helps spot allocation-heavy code.");

    ui->lbSmallTotalAllocationHelp->setPixmap(pixmap);
    ui->lbSmallTotalAllocationHelp->setToolTip("Total allocated memory (in MB) below which a test is considered memory-light. Useful for skipping trivial tests.");

    ui->lbMemoryJumpThresholdHelp->setPixmap(pixmap);
    ui->lbMemoryJumpThresholdHelp->setToolTip("The minimum percentage increase between two snapshots required to flag a sudden memory usage spike."
                                              " Expressed as a percentage.");

    ui->lbLargeMemoryThresholdHelp->setPixmap(pixmap);
    ui->lbLargeMemoryThresholdHelp->setToolTip("Defines what is considered extremely large memory usage overall (in MB)."
                                               " Helps filter out heavy tests or data-heavy functions.");

    ui->lbMemoryFreeThresholdHelp->setPixmap(pixmap);
    ui->lbMemoryFreeThresholdHelp->setToolTip("The minimum drop in memory (in KB) between snapshots that indicates a significant deallocation event.");

    ui->lbFragmentationThresholdHelp->setPixmap(pixmap);
    ui->lbFragmentationThresholdHelp->setToolTip("The ratio of extra heap memory (unused but reserved) to actual used heap memory,"
                                                 " above which fragmentation is suspected. Expressed as a percentage.");

    ui->lbStabilizationSensitivityHelp->setPixmap(pixmap);
    ui->lbStabilizationSensitivityHelp->setToolTip("Number of consecutive snapshots to check after a detected memory jump to confirm if heap memory usage has stabilized.");

    this->setFixedSize(this->size());
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
    ui->sbStabilizationSensitivity->setValue(thresholds->stabilizationWindowSize);
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
    massifAnalyzerThresholds->stabilizationWindowSize = ui->sbStabilizationSensitivity->text().toInt();

    emit thresholdsChanged(massifAnalyzerThresholds);
}

