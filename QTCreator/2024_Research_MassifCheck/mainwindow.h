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

private slots:
    void on_btLoadBinary_clicked();

    void on_btRunAnalysis_clicked();

private:
    Ui::MainWindow *ui;
    MassifRunner *massifRunner;
};
#endif // MAINWINDOW_H
