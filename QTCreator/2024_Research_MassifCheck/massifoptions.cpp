#include "massifoptions.h"

MassifOptions::MassifOptions(QObject *parent)
    : QObject{parent}
{}

QString MassifOptions::makeAdditionalArguments()
{
    QString aditionalArguments;

    QString arg = "--heap=";
    aditionalArguments += includeHeapProfiling ? "yes " : "no ";

    arg = "--stack=";
    aditionalArguments += includeStackProfiling ? "yes " : "no ";

    arg = "--time-unit=";
    if ( timeUnit == INSTRUCTIONS) { arg += "i "; }
    else if ( timeUnit == MILISECONDS ) { arg += "ms "; }
    else { arg += "B "; }
    aditionalArguments += arg;

    for ( auto fn : ignoreFunctions ){
        // TODO search if the function really exists

        aditionalArguments += "--ignore-fn=" + fn + " ";
    }

    return aditionalArguments;
}

Q_DECLARE_METATYPE(MassifOptions)
