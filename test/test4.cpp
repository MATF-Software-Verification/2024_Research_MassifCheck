#include <iostream>
#include <vector>
#include <cstdlib>

int main() {
    std::vector<void*> allocations;


    for (int i = 0; i < 10000; ++i) {
        allocations.push_back(malloc(32));
    }


    for (int i = 0; i < 10000; i += 2) {
        free(allocations[i]);
        allocations[i] = nullptr;
    }

    std::cout << "Finished allocations and frees. Check your massif snapshot now." << std::endl;


    for (void* ptr : allocations) {
        if (ptr) free(ptr);
    }

    return 0;
}