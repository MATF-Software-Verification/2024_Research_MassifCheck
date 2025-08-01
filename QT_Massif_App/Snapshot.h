#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QVector>
#include "AllocationEntry.h"

struct Snapshot{
    int snapshot;
    long time;
    long mem_heap_B;
    long mem_heap_extra_B;
    long mem_stacks_B;

    QVector<AllocationEntry> allocations; //new field for detailed allocations
};

#endif // SNAPSHOT_H
