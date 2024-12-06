#include "DataCollector.h"
#include <cstdlib>
#include <ctime>

DataCollector::DataCollector() {
    // Initialize labels for 24 points
    measurementLabels = {
        "Point 1: Head", "Point 2: Neck", "Point 3: Left Shoulder", "Point 4: Right Shoulder",
        "Point 5: Left Arm", "Point 6: Right Arm", "Point 7: Heart", "Point 8: Lungs",
        "Point 9: Liver", "Point 10: Stomach", "Point 11: Left Kidney", "Point 12: Right Kidney",
        "Point 13: Bladder", "Point 14: Small Intestine", "Point 15: Large Intestine",
        "Point 16: Spleen", "Point 17: Gallbladder", "Point 18: Pancreas",
        "Point 19: Left Leg", "Point 20: Right Leg", "Point 21: Left Foot",
        "Point 22: Right Foot", "Point 23: Spine", "Point 24: Nervous System"
    };
}

std::vector<float> DataCollector::collectData() {
    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    rawData.clear();
    // Simulate collecting data for 24 points based on the Ryodoraku chart's range (e.g., 10 to 150)
    for (int i = 0; i < 24; ++i) {
        // Generate random value in range [10, 150]
        float value = static_cast<float>(std::rand() % 141 + 10);
        rawData.push_back(value);
    }

    return rawData;
}

std::vector<std::string> DataCollector::getMeasurementLabels() const {
    return measurementLabels;
}