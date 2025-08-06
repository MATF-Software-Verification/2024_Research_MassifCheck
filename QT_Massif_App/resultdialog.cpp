#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResultDialog)
{
    ui->setupUi(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::Monospace);

    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));

    ui->plainTextEdit->setFont(font);
    ui->plainTextEdit_2->setFont(font);

    this->setFixedSize(this->size());
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setText(const QString &text)
{
    ui->plainTextEdit->setPlainText(text);
}

void ResultDialog::setGraph(const QString &graph)
{
    ui->plainTextEdit_2->setPlainText(graph);
}
