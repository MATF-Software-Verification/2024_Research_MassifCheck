#include "massifoptions.h"

MassifOptions::MassifOptions(QObject *parent)
    : QObject{parent}
{}

QString MassifOptions::makeAdditionalArguments()
{
    QString aditionalArguments;

    aditionalArguments += "--heap=";
    aditionalArguments += includeHeapProfiling ? "yes " : "no ";

    aditionalArguments += "--stacks=";
    aditionalArguments += includeStackProfiling ? "yes " : "no ";

    aditionalArguments += "--time-unit=";
    if ( timeUnit == INSTRUCTIONS) { aditionalArguments += "i "; }
    else if ( timeUnit == MILISECONDS ) { aditionalArguments += "ms "; }
    else { aditionalArguments += "B "; }

    return aditionalArguments;
}

Q_DECLARE_METATYPE(MassifOptions)
