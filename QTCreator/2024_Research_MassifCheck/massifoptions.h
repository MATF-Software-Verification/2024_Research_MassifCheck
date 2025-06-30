#ifndef MASSIFOPTIONS_H
#define MASSIFOPTIONS_H

#include <QObject>
#include "TimeUnit.h"

class MassifOptions : public QObject
{
    Q_OBJECT
public:
    explicit MassifOptions(QObject *parent = nullptr);

    bool includeHeapProfiling = true;
    bool includeStackProfiling = false;
    TimeUnit timeUnit = INSTRUCTIONS;
    uint maxSnapshots = 100;

    QString makeAdditionalArguments();


signals:

};

#endif // MASSIFOPTIONS_H
