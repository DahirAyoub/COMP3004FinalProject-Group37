#include "DataCollector.h"
#include <cstdlib>
#include <ctime>

// Collects simulated raw health data
std::vector<float> DataCollector::collectData() {
    // Seed random number generator for consistent randomness
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::vector<float> rawData;
    // Simulate collecting data at 24 body points (RaDoTech standard)
    for (int i = 0; i < 24; ++i) {
        // Generate random electrical readings (e.g., between 0.5 and 10.0)
        float reading = static_cast<float>(std::rand() % 1000) / 100.0f + 0.5f;
        rawData.push_back(reading);
    }
    return rawData;
}