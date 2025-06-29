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

void MassifAnalyzer::detectMemoryLeaks(const QVector<Snapshot>& snapshots) {
    const double MEMORY_JUMP_THRESHOLD = 0.5; // 50%
    const qint64 LARGE_MEMORY_THRESHOLD = 1000000000; // 1 GB
    const qint64 BYTES_TO_MB = 1024 * 1024;

    Snapshot previousSnapshot;
    bool hasPreviousSnapshot = false;

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
                qDebug() << "Info: Heap memory jump between snapshot"
                         << previousSnapshot.snapshot << "and snapshot"
                         << snap.snapshot << "is" << heapJump * 100 << "%";

                if (isMemoryStabilized(snapshots, i)) {
                    qDebug() << "Note: Memory stabilized after snapshot" << snap.snapshot;
                } else {
                    qDebug() << "Warning: Memory continues to grow after snapshot" << snap.snapshot;
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
                qDebug() << "Info: Stack memory jump between snapshot"
                         << previousSnapshot.snapshot << "and snapshot"
                         << snap.snapshot << "is" << stackJump * 100 << "%";
            }
        }

        if (snap.mem_heap_B > LARGE_MEMORY_THRESHOLD) {
            qDebug() << "Warning: Large heap memory detected in snapshot"
                     << snap.snapshot << ":" << snap.mem_heap_B / BYTES_TO_MB << "MB";
        }

        if (snap.mem_stacks_B > LARGE_MEMORY_THRESHOLD) {
            qDebug() << "Warning: Large stack memory detected in snapshot"
                     << snap.snapshot << ":" << snap.mem_stacks_B / BYTES_TO_MB << "MB";
        }

        // Update previous only if valid
        if (snap.mem_heap_B != 0 || snap.mem_stacks_B != 0 || snap.mem_heap_extra_B != 0) {
            previousSnapshot = snap;
            hasPreviousSnapshot = true;
        }
    }
}
