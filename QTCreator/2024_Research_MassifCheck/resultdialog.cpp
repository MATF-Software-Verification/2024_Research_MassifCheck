#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setText(const QString &text)
{
    ui->plainTextEdit->setPlainText(text);
}
