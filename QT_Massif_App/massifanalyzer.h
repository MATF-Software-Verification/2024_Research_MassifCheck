#ifndef MASSIFANALYZER_H
#define MASSIFANALYZER_H

#include "snapshot.h"
#include <QObject>
#include <QVector>
#include "snapshot.h"
#include <QMap>
#include "functionallocsummary.h"

class MassifAnalyzer
{
public:
    MassifAnalyzer();

    bool isMemoryStabilized(const QVector<Snapshot>& snapshots, int currentIndex, int windowSize = 3);
    QVector<FunctionAllocSummary> analyzeAllocationsPerFunction(const Snapshot& snapshot);
    QString detectMemoryLeaks(const QVector<Snapshot>& snapshots);
};

#endif // MASSIFANALYZER_H
