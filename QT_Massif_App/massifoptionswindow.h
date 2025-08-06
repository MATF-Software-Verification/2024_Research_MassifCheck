#ifndef MASSIFOPTIONSWINDOW_H
#define MASSIFOPTIONSWINDOW_H

#include <QDialog>
#include "massifoptions.h"

namespace Ui {
class MassifOptionsWindow;
}

class MassifOptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MassifOptionsWindow(QWidget *parent = nullptr);
    ~MassifOptionsWindow();

    void setOptionsFields(MassifOptions *options);

private slots:
    void on_buttonBox_accepted();

signals:
    void optionsChanged(MassifOptions* options);

private:
    Ui::MassifOptionsWindow *ui;
};

#endif // MASSIFOPTIONSWINDOW_H
