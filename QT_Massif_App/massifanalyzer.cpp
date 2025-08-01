#include "massifanalyzer.h"
#include <QDebug>

MassifAnalyzer::MassifAnalyzer(){}


bool MassifAnalyzer::isMemoryStabilized(const QVector<Snapshot>& snapshots, int currentIndex, int windowSize) {
    if (currentIndex < 0 || currentIndex + windowSize >= snapshots.size())
        return false;

    const Snapshot& currentSnap = snapshots[currentIndex];

    for (int i = 1; i <= windowSize && currentIndex + i < snapshots.size(); ++i) {
        if (currentSnap.mem_heap_B < snapshots[currentIndex + i].mem_heap_B) {
            return false;
        }
    }
    return true;
}

QString MassifAnalyzer::detectMemoryLeaks(const QVector<Snapshot>& snapshots) {
    const double MEMORY_JUMP_THRESHOLD = 0.5; // 50%
    const qint64 LARGE_MEMORY_THRESHOLD = 1000000000; // 1 GB
    const qint64 BYTES_TO_MB = 1024 * 1024;
    const qint64 MEMORY_FREE_THRESHOLD = 4 * 1024; // 4 KB
    const double FRAGMENTATION_THRESHOLD = 0.10; // 10%

    Snapshot previousSnapshot;
    bool hasPreviousSnapshot = false;

    QString result;

    for (int i = 0; i < snapshots.size(); ++i) {
        const Snapshot& snap = snapshots[i];

        if (hasPreviousSnapshot) {
            double heapJump = 0.0;

            if (previousSnapshot.mem_heap_B != 0) {
                qint64 heapDiff = snap.mem_heap_B - previousSnapshot.mem_heap_B;
                if (heapDiff > 0) {
                    heapJump = static_cast<double>(heapDiff) / previousSnapshot.mem_heap_B;
                }
            } else if (snap.mem_heap_B > LARGE_MEMORY_THRESHOLD) {
                heapJump = 1.0;
            }

            if (heapJump >= MEMORY_JUMP_THRESHOLD) {
                result += QString("Info: Heap memory jump between snapshot %1 and snapshot %2 is %3%\n")
                    .arg(previousSnapshot.snapshot)
                    .arg(snap.snapshot)
                    .arg(heapJump * 100, 0, 'f', 2);

                if (isMemoryStabilized(snapshots, i)) {
                    result += QString("Note: Memory stabilized after snapshot %1\n")
                        .arg(snap.snapshot);
                } else {
                    result += QString("Warning: Memory continues to grow after snapshot %1\n")
                        .arg(snap.snapshot);
                }
            }

            double stackJump = 0.0;

            if (previousSnapshot.mem_stacks_B != 0) {
                qint64 stackDiff = snap.mem_stacks_B - previousSnapshot.mem_stacks_B;
                if (stackDiff > 0) {
                    stackJump = static_cast<double>(stackDiff) / previousSnapshot.mem_stacks_B;
                }
            } else if (snap.mem_stacks_B > LARGE_MEMORY_THRESHOLD) {
                stackJump = 1.0;
            }

            if (stackJump > MEMORY_JUMP_THRESHOLD) {
                result += QString("Info: Stack memory jump between snapshot %1 and snapshot %2 is %3%\n")
                    .arg(previousSnapshot.snapshot)
                    .arg(snap.snapshot)
                    .arg(stackJump * 100, 0, 'f', 2);
            }
        }

        if (snap.mem_heap_B > LARGE_MEMORY_THRESHOLD) {
            result += QString("Warning: Large heap memory detected in snapshot %1: %2 MB\n")
                .arg(snap.snapshot)
                .arg(snap.mem_heap_B / BYTES_TO_MB);
        }

        if (snap.mem_stacks_B > LARGE_MEMORY_THRESHOLD) {
            result += QString("Warning: Large stack memory detected in snapshot %1: %2 MB\n")
                .arg(snap.snapshot)
                .arg(snap.mem_stacks_B / BYTES_TO_MB);
        }

        // Update previous only if valid
        if (snap.mem_heap_B != 0 || snap.mem_stacks_B != 0 || snap.mem_heap_extra_B != 0) {
            previousSnapshot = snap;
            hasPreviousSnapshot = true;
        }

        if (snap.mem_heap_B > 0) { // da ne deliš sa nulom
            double fragmentationRatio = static_cast<double>(snap.mem_heap_extra_B) / snap.mem_heap_B;

            if (fragmentationRatio > FRAGMENTATION_THRESHOLD) {
                result += QString("Warning: Possible heap fragmentation in snapshot %1: extra memory is %2% of heap\n")
                .arg(snap.snapshot)
                    .arg(fragmentationRatio * 100, 0, 'f', 2);
            }
        }

        if (i == snapshots.size() - 1){
            if (snap.mem_heap_B > MEMORY_FREE_THRESHOLD) {
                result += QString("Warning: Memory not fully freed at the end! Heap usage: %1 bytes\n")
                .arg(snap.mem_heap_B);
            } else {
                result += QString("Info: Memory fully freed at the end.\n");
            }
        }

        // if (!snap.allocations.isEmpty()) {
        //     result += QString("Top allocations in snapshot %1\n").arg(snap.snapshot);
        //     for (const AllocationEntry& alloc : snap.allocations) {
        //         result += QString("%1 bytes in %2 at %3 : %4\n")
        //                       .arg(alloc.bytes)
        //                       .arg(alloc.function)
        //                       .arg(alloc.sourceFile)
        //                       .arg(alloc.line);
        //     }
        // }
    }

    return result;
}

QString MassifAnalyzer::generateFunctionAllocationReport(const QMap<QString, FunctionAllocSummary>& functionSummary) {
    const qint64 HIGH_MEMORY_THRESHOLD = 100 * 1024 * 1024; // 100 MB
    const int HIGH_ALLOCATION_COUNT = 10;
    const qint64 SMALL_TOTAL_ALLOCATION = 5 * 1024 * 1024; // 5 MB

    QString result;

    for (auto it = functionSummary.constBegin(); it != functionSummary.constEnd(); ++it) {
        const FunctionAllocSummary& summary = it.value();

        result += QString("Function '%1' allocated total %2 bytes in %3 allocations.\n")
                      .arg(summary.function)
                      .arg(summary.totalBytes)
                      .arg(summary.count);

        // Warn if a function allocates a lot of memory
        if (summary.totalBytes > HIGH_MEMORY_THRESHOLD) {
            result += QString("Warning: Function '%1' is responsible for a large memory allocation (over 100MB).\n")
            .arg(summary.function);
        }

        if (summary.count > HIGH_ALLOCATION_COUNT) {
            if (summary.totalBytes < SMALL_TOTAL_ALLOCATION) {
                result += QString("Note: Function '%1' performs many small allocations (%2); consider optimizing with preallocation or pooling.\n")
                .arg(summary.function)
                    .arg(summary.count);
            } else {
                result += QString("Note: Function '%1' performs many allocations (%2); consider checking for inefficiencies.\n")
                .arg(summary.function)
                    .arg(summary.count);
            }
        }

        result += "\n";
    }

    return result;
}
