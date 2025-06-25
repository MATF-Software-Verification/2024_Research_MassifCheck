#ifndef MASSIFOPTIONSWINDOW_H
#define MASSIFOPTIONSWINDOW_H

#include <QDialog>

namespace Ui {
class MassifOptionsWindow;
}

class MassifOptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MassifOptionsWindow(QWidget *parent = nullptr);
    ~MassifOptionsWindow();

private:
    Ui::MassifOptionsWindow *ui;
};

#endif // MASSIFOPTIONSWINDOW_H
