#include "parser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

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
    QRegularExpression allocRegex(R"(n\d+:\s+(\d+)\s+0x[0-9A-Fa-f]+:\s+(.+)\s+\((.+):(\d+)\))");

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match;

        if (line.contains(headerRegex, &match)) {
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
        else if (line.startsWith("n")){
            QRegularExpressionMatch allocMatch = allocRegex.match(line);
            if (allocMatch.hasMatch()){
                AllocationEntry entry;
                entry.bytes = allocMatch.captured(1).toLongLong();
                entry.function = allocMatch.captured(2);
                entry.sourceFile = allocMatch.captured(3);
                entry.line = allocMatch.captured(4).toInt();

                snapshot.allocations.append(entry);
            }
        }
    }

    if (snapshot.snapshot >= 0) {
        snapshots.append(snapshot);
    }

    file.close();
    return {header, snapshots};
}
