#ifndef MASSIFANALYZERTHRESHOLDSWINDOW_H
#define MASSIFANALYZERTHRESHOLDSWINDOW_H

#include <QDialog>
#include "massifanalyzerthresholds.h"

namespace Ui {
class MassifAnalyzerThresholdsWindow;
}

class MassifAnalyzerThresholdsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MassifAnalyzerThresholdsWindow(QWidget *parent = nullptr);
    ~MassifAnalyzerThresholdsWindow();

    void setThresholdsFields(MassifAnalyzerThresholds *thresholds);

private slots:
    void on_buttonBox_accepted();

signals:
    void thresholdsChanged(MassifAnalyzerThresholds *thresholds);
private:
    Ui::MassifAnalyzerThresholdsWindow *ui;
};

#endif // MASSIFANALYZERTHRESHOLDSWINDOW_H
