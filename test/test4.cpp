#include <iostream>
#include <vector>
#include <stdexcept>

void dangerousAllocation(int iteration) {
    char* leak = new char[1024 * 100];  // 100 KB leak
    if (iteration % 100 == 0) {
        throw std::runtime_error("Simulated failure");
    }
    delete[] leak;
}

int main() {
    for (int i = 1; i <= 1000; ++i) {
        try {
            dangerousAllocation(i);
        } catch (const std::exception& e) {
            std::cerr << "Caught exception at iteration " << i << ": " << e.what() << "\n";
        }
    }

    return 0;
}