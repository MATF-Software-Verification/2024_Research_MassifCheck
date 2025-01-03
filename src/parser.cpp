#include "parser.h"
#include <fstream>
#include <regex>
//#include <stdexcept>


std::pair<std::map<std::string, std::string>, std::vector<Snapshot>>parseMassifFile(const std::string &filePath){
	std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::map<std::string, std::string> header;
    std::vector<Snapshot> snapshots;
    std::string line;
    Snapshot snapshot;
    bool hasSnapshot=false;

    std::regex headerRegex(R"((\w+):\s*(.+))");
    std::regex snapshotRegex(R"(snapshot=(\d+))");
    std::regex valueRegex(R"((\w+)=([\d]+))");

    while (std::getline(file, line)) {
        std::smatch match;

        if (std::regex_match(line, match, headerRegex)) {
            header[match[1]] = match[2];
        }
        else if (std::regex_match(line, match, snapshotRegex)) {
            if (hasSnapshot) {
                snapshots.push_back(snapshot);
            }
            snapshot = {}; 
            snapshot.snapshot = std::stoi(match[1]);
            hasSnapshot=true;
        }
        else if (std::regex_match(line, match, valueRegex)) {
            std::string key = match[1];
            long value = std::stol(match[2]);
            if (key == "time") snapshot.time = value;
            else if (key == "mem_heap_B") snapshot.mem_heap_B = value;
            else if (key == "mem_heap_extra_B") snapshot.mem_heap_extra_B = value;
            else if (key == "mem_stacks_B") snapshot.mem_stacks_B = value;
        }
    }

    if (snapshot.snapshot >= 0) {
        snapshots.push_back(snapshot);
    }

    file.close();
    return {header, snapshots};
}
