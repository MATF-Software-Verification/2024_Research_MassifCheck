#include "parser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <iostream>

Parser::Parser() {}

std::pair<QMap<QString, QString>, QVector<Snapshot>> Parser::parseMassifFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return {};
    }

    QMap<QString, QString> header;
    QVector<Snapshot> snapshots;
    Snapshot snapshot;
    bool hasSnapshot = false;

    QRegularExpression headerRegex(R"((\w+):\s*(.+))");
    QRegularExpression snapshotRegex(R"(snapshot=(\d+))");
    QRegularExpression valueRegex(R"((\w+)=([\d]+))");
    QRegularExpression detailedAllocRegex(R"(n\d+:\s+(\d+)\s+0x[0-9A-Fa-f]+:\s+([\w:<>~]+)\s+\(([^:]+):(\d+)\))");
    QRegularExpression summaryAllocRegex(R"(n\d+:\s+(\d+)\s+\(.+\))");

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match;

        if (line.trimmed().startsWith("n")) {
            QRegularExpression detailedAllocRegex(R"(n\d+:\s+(\d+)\s+0x[0-9A-Fa-f]+:\s+(.+)\s+\((.+):(\d+)\))");
            // (npr. "n1: 4000 (heap allocation functions) malloc/new/new[], --alloc-fns, etc.")
            QRegularExpression summaryAllocRegex(R"(n\d+:\s+(\d+)\s+\(.+\))");
            static QRegularExpression stdlibFunctionRegex(R"(^(std::|__|_Z|boost::))");

            QRegularExpressionMatch match;

            if ((match = detailedAllocRegex.match(line)).hasMatch()) {
                AllocationEntry entry;
                entry.bytes = match.captured(1).toLongLong();
                entry.function = match.captured(2);
                entry.sourceFile = match.captured(3);
                entry.line = match.captured(4).toInt();

                //std::cout << entry.bytes << " bytes, " << entry.function.toStdString()
                //          << " " << entry.sourceFile.toStdString() << ":" << entry.line << std::endl;

                bool isFromStdLib = stdlibFunctionRegex.match(entry.function).hasMatch();
                bool hasValidSourceFile = !entry.sourceFile.isEmpty()
                                          && (entry.sourceFile.endsWith(".cpp") || entry.sourceFile.endsWith(".cc")
                                              || entry.sourceFile.endsWith(".c") || entry.sourceFile.endsWith(".hpp")
                                              || entry.sourceFile.endsWith(".h"));

                if (isFromStdLib || !hasValidSourceFile) {
                    continue; // skip this entry
                }

                snapshot.allocations.append(entry);
            }
            else if ((match = summaryAllocRegex.match(line)).hasMatch()) {
                AllocationEntry entry;
                entry.bytes = match.captured(1).toLongLong();
                entry.function = "(summary)";
                entry.sourceFile = "";
                entry.line = 0;

                //std::cout << entry.bytes << " bytes, summary allocation line" << std::endl;

                snapshot.allocations.append(entry);
            }
            else {
                qDebug() << "Failed to match allocation line:" << line;
            }
        }
        else if (line.contains(headerRegex, &match)) {
            header[match.captured(1)] = match.captured(2);
        }
        else if (line.contains(snapshotRegex, &match)) {
            if (hasSnapshot) {
                snapshots.append(snapshot);
            }
            snapshot = {};
            snapshot.snapshot = match.captured(1).toInt();
            hasSnapshot = true;
        }
        else if (line.contains(valueRegex, &match)) {
            QString key = match.captured(1);
            qint64 value = match.captured(2).toLongLong();
            if (key == "time") snapshot.time = value;
            else if (key == "mem_heap_B") snapshot.mem_heap_B = value;
            else if (key == "mem_heap_extra_B") snapshot.mem_heap_extra_B = value;
            else if (key == "mem_stacks_B") snapshot.mem_stacks_B = value;
        }

    }

    if (snapshot.snapshot >= 0) {
        snapshots.append(snapshot);
    }

    file.close();
    return {header, snapshots};
}


// this function groups allocations with functions summing the amount of memory allocated in those functions
// the problem is right now we have every function mentioned, and in more complex programs that will include STL etc..
QMap<QString, FunctionAllocSummary> Parser::summarizeAllocationsByFunction(const QVector<Snapshot>& snapshots) {
    QMap<QString, FunctionAllocSummary> functionTotals;

    for (const auto& snapshot : snapshots) {
        for (const auto& alloc : snapshot.allocations) {
            if (alloc.function == "(summary)") continue; // skip summary lines

            auto& summary = functionTotals[alloc.function];
            summary.function = alloc.function;
            summary.totalBytes += alloc.bytes;
            summary.count += 1;
        }
    }

    return functionTotals;
}
