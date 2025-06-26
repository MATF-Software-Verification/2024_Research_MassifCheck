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

}

