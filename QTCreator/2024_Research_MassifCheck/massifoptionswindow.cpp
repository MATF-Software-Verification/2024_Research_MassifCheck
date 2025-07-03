#include "massifoptionswindow.h"
#include "ui_massifoptionswindow.h"
#include <QStyle>

MassifOptionsWindow::MassifOptionsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MassifOptionsWindow)
{
    ui->setupUi(this);
    QPixmap pixmap = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16, 16);
    ui->lbHeapProfilingHelp->setPixmap(pixmap);
    ui->lbStackProfilingHelp->setPixmap(pixmap);
    ui->lbTimeUnitHelp->setPixmap(pixmap);
    ui->lbMaxSnapshotsHelp->setPixmap(pixmap);
    //ui->btMassifOptions->setToolTip("Configure Massif settings");
    ui->lbHeapProfilingHelp->setToolTip("Specifies whether heap profiling should be done.");
    ui->lbStackProfilingHelp->setToolTip("Specifies whether stack profiling should be done.");
    ui->lbTimeUnitHelp->setToolTip("The time unit used for the profiling. "
                               "There are three possibilities: instructions executed (i), which is good for most cases; "
                               "real (wallclock) time (ms, i.e. milliseconds), which is sometimes useful; "
                               "and bytes allocated/deallocated on the heap and/or stack (B), which is useful for very short-run programs, "
                               "and for testing purposes, because it is the most reproducible across different machines.");
    ui->lbMaxSnapshotsHelp->setToolTip("The maximum number of snapshots recorded. If set to N, "
                                   "for all programs except very short-running ones, the final number of snapshots will be between N/2 and N.");
}

MassifOptionsWindow::~MassifOptionsWindow()
{
    delete ui;
}

void MassifOptionsWindow::on_buttonBox_accepted()
{
    MassifOptions* options = new MassifOptions();
    options->includeHeapProfiling = ui->rbHeapYes->isChecked();
    options->includeStackProfiling = ui->rbStackYes->isChecked();
    if ( ui->rbTimeUnitMs->isChecked() ){
        options->timeUnit = MILISECONDS;
    } else if ( ui->rbTimeUnitB->isChecked() ){
        options->timeUnit = BYTES;
    } else {
        options->timeUnit = INSTRUCTIONS;
    }
    options->maxSnapshots = ui->sbMaxSnapshots->value();

    // TODO set functions to ignore based on what is written in tbIgnore functions

     emit optionsChanged(options);
}


