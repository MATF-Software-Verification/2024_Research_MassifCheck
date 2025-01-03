#ifndef MEMORY_ANALYSIS
#define MEMORY_ANALYSIS

#include "snapshot.h"
#include <vector>
#include <iostream>

bool isMemoryStabilized(const std::vector<Snapshot>& snapshots, size_t currentIndex, size_t windowSize = 3);
void detectMemoryLeaks(const std::vector<Snapshot>& snapshots);

#endif