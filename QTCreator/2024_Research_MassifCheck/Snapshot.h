#ifndef SNAPSHOT_H
#define SNAPSHOT_H

struct Snapshot{
    int snapshot;
    long time;
    long mem_heap_B;
    long mem_heap_extra_B;
    long mem_stacks_B;
};

#endif // SNAPSHOT_H
