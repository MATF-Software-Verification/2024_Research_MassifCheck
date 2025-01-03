#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <cmath>

struct Snapshot{
	int snapshot;
	long time;
	long mem_heap_B;
	long mem_heap_extra_B;
	long mem_stacks_B;
};


std::pair<std::map<std::string, std::string>, std::vector<Snapshot>>parseMassifFile(const std::string &filePath){
	std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::map<std::string, std::string> header;
    std::vector<Snapshot> snapshots;
    std::string line;
    Snapshot snapshot;

    std::regex headerRegex(R"((\w+):\s*(.+))");
    std::regex snapshotRegex(R"(snapshot=(\d+))");
    std::regex valueRegex(R"((\w+)=([\d]+))");

    while (std::getline(file, line)) {
        std::smatch match;

        if (std::regex_match(line, match, headerRegex)) {
            header[match[1]] = match[2];
        }
        else if (std::regex_match(line, match, snapshotRegex)) {
            if (snapshot.snapshot >= 0) {
                snapshots.push_back(snapshot);
            }
            snapshot = {}; 
            snapshot.snapshot = std::stoi(match[1]);
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


void detectMemoryLeaks(const std::vector<Snapshot>& snapshots) {
    const double MEMORY_JUMP_TRESHOLD = 0.5; // 50% - prag za nagli skok
    const double LARGE_MEMORY_TRESHOLD = 1000000000; // 1GB - prag za veliku memoriju

    Snapshot previousSnapshot;

    for(size_t i = 0; i < snapshots.size(); i++){

        const auto& snap = snapshots[i];

        if(previousSnapshot.snapshot>=0){
            double heapJump = std::abs(static_cast<double> (snap.mem_heap_B - previousSnapshot.mem_heap_B)/previousSnapshot.mem_heap_B);
            double stackJump = std::abs(static_cast<double> (snap.mem_stacks_B - previousSnapshot.mem_stacks_B )/previousSnapshot.mem_stacks_B);

            if(heapJump > MEMORY_JUMP_TRESHOLD){
                std::cout << "Info: Heap memory jump between snapshot " 
                    << previousSnapshot.snapshot
                    << " and snapshot "
                    << snap.snapshot
                    << " is " << heapJump*100 << "%\n" ;
            }

            if(stackJump > MEMORY_JUMP_TRESHOLD){
                std::cout << "Info: Stack memory jump between snapshot " 
                    << previousSnapshot.snapshot
                    << " and snapshpt "
                    << snap.snapshot
                    << " is " << stackJump*100 << "%\n";
            }
        }
        
        const long BYTES_TO_MB = 1024 * 1024;

        if(snap.mem_heap_B > LARGE_MEMORY_TRESHOLD){
            std::cout << "Warning: Large heap memory detected in snapshot " << snap.snapshot
                << ": " << snap.mem_heap_B / BYTES_TO_MB << "MB\n";
        }

        if(snap.mem_heap_B > LARGE_MEMORY_TRESHOLD){
            std::cout << "Warning: Large stack memory detected in snapshot " << snap.snapshot
                << ": " << snap.mem_stacks_B / BYTES_TO_MB << "MB\n";
        }

        previousSnapshot = snap;        
    }

}



int main(int argc, char* argv[]){

	if(argc < 2){
		std::cerr<< "Usage: " << argv[0] << " <massif_out_file>\n";
		return 1;
	}

	std::string file = argv[1];

	try{
		auto[header, snapshots] = parseMassifFile(file);
		for (const auto &snap : snapshots) {
            std::cout << "Snapshot: " << snap.snapshot
                      << ", Time: " << snap.time
                      << ", Heap: " << snap.mem_heap_B
                      << ", Extra Heap: " << snap.mem_heap_extra_B
                      << ", Stacks: " << snap.mem_stacks_B << "\n";
        }

        detectMemoryLeaks(snapshots);

	} catch (const std::exception &e){
		std::cerr << "Error: " << e.what() <<"\n";
	}
	return 0;
}


