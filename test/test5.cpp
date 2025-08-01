#include <iostream>
#include <vector>
#include <string>

void largeAllocator() {
    // Allocates over 100 MB -> should trigger "Warning: large memory allocation"
    std::vector<char> bigBlock(150 * 1024 * 1024); // 150 MB
}

void manySmallAllocations() {
    std::vector<int*> ptrs;
    for (int i = 0; i < 1500; ++i) {
        ptrs.push_back(new int(i)); // svaka iteracija pravi novu malloc alokaciju
    }
}

void bothHeavyAndFrequent() {
    // Allocates frequently and with large total size -> triggers both warnings and notes
    std::vector<std::string> heavy;
    for (int i = 0; i < 2000; ++i) {
        heavy.emplace_back(std::string(1024, 'a')); // 1 KB * 2000 = 2 MB total, many allocations
    }
}

void negligibleAllocator() {
    // Allocates very little memory -> should not trigger any warning or note
    std::string s = "hello world";
}

int main() {
    largeAllocator();
    manySmallAllocations();
    bothHeavyAndFrequent();
    negligibleAllocator();

    return 0;
}