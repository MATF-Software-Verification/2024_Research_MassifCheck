#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "massifrunner.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void modeValudeChanged(Mode mode);

private slots:
    void on_btLoadFile_clicked();

    void on_btExecute_clicked();

    void on_rbBinary_toggled(bool checked){
        if (checked){
            modeValudeChanged(BINARY);
        }
    };

    void on_rbCompile_toggled(bool checked){
        if (checked){
            modeValudeChanged(COMPILE);
        }
    };

    void on_rbOuput_toggled(bool checked){
        if (checked){
            modeValudeChanged(OUTPUT);
        }
    };

private:
    Ui::MainWindow *ui;
    MassifRunner *massifRunner;
};
#endif // MAINWINDOW_H
