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
    std::vector<QString> ignoreFunctions;

    QString makeAdditionalArguments();


signals:

};

#endif // MASSIFOPTIONS_H
