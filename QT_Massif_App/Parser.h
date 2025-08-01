#ifndef PARSER_H
#define PARSER_H

#include "snapshot.h"
#include <QObject>
#include <QMap>
#include <QVector>
#include <QString>
#include "functionallocsummary.h"

class Parser
{
public:
    Parser();
    std::pair<QMap<QString, QString>, QVector<Snapshot>> parseMassifFile(const QString &filePath);
    QMap<QString, FunctionAllocSummary> summarizeAllocationsByFunction(const QVector<Snapshot>& snapshots);
};

#endif // PARSER_H
