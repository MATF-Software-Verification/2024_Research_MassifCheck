#include "parser.h"
#include "memory_analysis.h"
#include <iostream>

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


