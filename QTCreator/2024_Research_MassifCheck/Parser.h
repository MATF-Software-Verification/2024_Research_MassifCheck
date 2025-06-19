#ifndef PARSER_H
#define PARSER_H

#include "snapshot.h"
#include <QObject>
#include <QMap>
#include <QVector>
#include <QString>

class Parser
{
public:
    Parser();
    std::pair<QMap<QString, QString>, QVector<Snapshot>> parseMassifFile(const QString &filePath);
};

#endif // PARSER_H
