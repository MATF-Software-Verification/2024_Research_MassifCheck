#include "massifoptionswindow.h"
#include "ui_massifoptionswindow.h"

MassifOptionsWindow::MassifOptionsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MassifOptionsWindow)
{
    ui->setupUi(this);
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
    if ( ui->rbTimeUnitMs ){
        options->timeUnit = MILISECONDS;
    } else if ( ui->rbTimeUnitB ){
        options->timeUnit = BYTES;
    }
    options->maxSnapshots = ui->sbMaxSnapshots->value();

    // TODO set functions to ignore based on what is written in tbIgnore functions

     emit optionsChanged(options);
}


