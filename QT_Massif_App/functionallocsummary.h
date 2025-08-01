#ifndef FUNCTIONALLOCSUMMARY_H
#define FUNCTIONALLOCSUMMARY_H

#include <QString>

struct FunctionAllocSummary {
    QString function;
    qint64 totalBytes = 0;
    int count = 0;
};


#endif // FUNCTIONALLOCSUMMARY_H
