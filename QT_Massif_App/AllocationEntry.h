#ifndef ALLOCATIONENTRY_H
#define ALLOCATIONENTRY_H

#include <QString>

struct AllocationEntry
{
    qint64 bytes;
    QString function;
    QString sourceFile;
    int line;
};

#endif // ALLOCATIONENTRY_H
