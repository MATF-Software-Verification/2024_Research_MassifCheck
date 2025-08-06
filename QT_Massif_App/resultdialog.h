#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QStyle>

namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    ~ResultDialog();

    void setText(const QString &text);
    void setGraph(const QString &graph);


private:
    Ui::ResultDialog *ui;
};

#endif // RESULTDIALOG_H
