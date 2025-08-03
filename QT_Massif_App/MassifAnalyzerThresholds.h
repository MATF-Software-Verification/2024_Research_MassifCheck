#ifndef MASSIFANALYZERTHRESHOLDS_H
#define MASSIFANALYZERTHRESHOLDS_H

#include <QObject>

const qint64 BYTES_TO_MB = 1024 * 1024;

class MassifAnalyzerThresholds : public QObject
{
    Q_OBJECT
public:
    explicit MassifAnalyzerThresholds(QObject *parent = nullptr);
    MassifAnalyzerThresholds(QObject *parent, MassifAnalyzerThresholds *thresholds);

    qint64 highMemoryThreshold = 100 * 1024 * 1024;        // 100 MB
    int highAllocationCount = 10;
    qint64 smallTotalAllocation = 5 * 1024 * 1024;         // 5 MB
    double memoryJumpThreshold = 0.5;                      // 50%
    qint64 largeMemoryThreshold = 1000000000;              // 1 GB
    qint64 memoryFreeThreshold = 4 * 1024;                 // 4 KB
    double fragmentationThreshold = 0.10;                  // 10%
    int stabilizationWindowSize = 3;

    void setThresholds(MassifAnalyzerThresholds *thresholds);

};

#endif // MASSIFANALYZERTHRESHOLDS_H
