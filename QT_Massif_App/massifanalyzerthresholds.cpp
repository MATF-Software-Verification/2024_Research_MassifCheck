#include "massifanalyzerthresholds.h"

MassifAnalyzerThresholds::MassifAnalyzerThresholds(QObject *parent)
    : QObject{parent}
{}

MassifAnalyzerThresholds::MassifAnalyzerThresholds(QObject *parent, MassifAnalyzerThresholds* thresholds)
    : QObject{parent}
    , highMemoryThreshold(thresholds->highMemoryThreshold)
    , highAllocationCount(thresholds->highAllocationCount)
    , smallTotalAllocation(thresholds->smallTotalAllocation)
    , memoryFreeThreshold(thresholds->memoryFreeThreshold)
    , memoryJumpThreshold(thresholds->memoryJumpThreshold)
    , largeMemoryThreshold(thresholds->largeMemoryThreshold)
    , fragmentationThreshold(thresholds->fragmentationThreshold)
    , stabilizationWindowSize(thresholds->stabilizationWindowSize)
{}

void MassifAnalyzerThresholds::setThresholds(MassifAnalyzerThresholds *thresholds)
{
    highMemoryThreshold = thresholds->highMemoryThreshold;
    highAllocationCount = thresholds->highAllocationCount;
    smallTotalAllocation = thresholds->smallTotalAllocation;
    memoryJumpThreshold = thresholds->memoryJumpThreshold;
    largeMemoryThreshold = thresholds->largeMemoryThreshold;
    memoryFreeThreshold = thresholds->memoryFreeThreshold;
    fragmentationThreshold = thresholds->fragmentationThreshold;
    stabilizationWindowSize = thresholds->stabilizationWindowSize;
}
