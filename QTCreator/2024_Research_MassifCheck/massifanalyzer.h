#ifndef MASSIFANALYZER_H
#define MASSIFANALYZER_H

#include "snapshot.h"
#include <QObject>
#include <QVector>

class MassifAnalyzer
{
public:
    MassifAnalyzer();

    bool isMemoryStabilized(const QVector<Snapshot>& snapshots, int currentIndex, int windowSize = 3);
    void detectMemoryLeaks(const QVector<Snapshot>& snapshots);

};

#endif // MASSIFANALYZER_H
