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

    aditionalArguments += "--max-snapshots=" + std::to_string(maxSnapshots) + " ";

    //adding this because we want to analyze every snapshot and make suggestions based on the informations
    aditionalArguments += "--detailed-freq=1 --alloc-fn=yes ";

    return aditionalArguments;
}

Q_DECLARE_METATYPE(MassifOptions)
