#include <iostream>
#include <deque>
#include <string>
#include <random>

class Logger {
    std::deque<std::string> buffer;
    size_t maxSize;

public:
    Logger(size_t maxSize) : maxSize(maxSize) {}

    void log(const std::string& message) {
        buffer.push_back(message);
        if (buffer.size() > maxSize) {
            // ne brišemo stare poruke — loš dizajn!
            // buffer.pop_front();  // <- simuliramo da ovo NE postoji
        }
    }

    void flush() {
        for (const auto& msg : buffer) {
            std::cout << msg << "\n";
        }
        buffer.clear();
    }
};

std::string randomMessage() {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    std::string result(1000, ' ');
    for (int i = 0; i < 1000; ++i)
        result[i] = charset[rand() % (sizeof(charset) - 1)];
    return result;
}

int main() {
    Logger logger(500);

    for (int i = 0; i < 10000; ++i) {
        logger.log("Entry " + std::to_string(i) + ": " + randomMessage());
        if (i % 1000 == 0)
            logger.flush(); // clear buffer
    }

    return 0;
}