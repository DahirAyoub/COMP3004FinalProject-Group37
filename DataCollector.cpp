#include "DataCollector.h"
#include <cmath> // For std::fabs
#include <ctime>

DataCollector::DataCollector() {
    // Initialize measurement labels for Ryodoraku points (H1-H6, F1-F6 for left and right sides)
    measurementLabels = {
        "H1 (Left)", "H2 (Left)", "H3 (Left)", "H4 (Left)", "H5 (Left)", "H6 (Left)",
        "H1 (Right)", "H2 (Right)", "H3 (Right)", "H4 (Right)", "H5 (Right)", "H6 (Right)",
        "F1 (Left)", "F2 (Left)", "F3 (Left)", "F4 (Left)", "F5 (Left)", "F6 (Left)",
        "F1 (Right)", "F2 (Right)", "F3 (Right)", "F4 (Right)", "F5 (Right)", "F6 (Right)"
    };

    // Seed the random generator
    generator.seed(static_cast<unsigned>(std::time(nullptr)));
}

// Helper function to generate synthetic data for a single Ryodoraku point
float DataCollector::generateMeasurementValue(float base, float deviation, bool isExcited, bool isInhibited) {
    // Modify the base value depending on excitation/inhibition
    if (isExcited) base += 20.0f; // Shift up for excitation
    if (isInhibited) base -= 20.0f; // Shift down for inhibition

    // Generate a random value using a normal distribution
    std::normal_distribution<float> distribution(base, deviation);
    float value = distribution(generator);

    // Clamp the value within the physiological range [10, 150]
    return std::fmax(10.0f, std::fmin(150.0f, value));
}

// Generate synthetic data for 24 Ryodoraku points
std::vector<float> DataCollector::collectData() {
    std::vector<float> rawData;

    // Base values for each Ryodoraku region
    float handBase = 50.0f;  // Hands (H1-H6)
    float footBase = 70.0f;  // Feet (F1-F6)
    float deviation = 10.0f; // Standard deviation for variability

    // Simulate data for each point
    for (int i = 0; i < 6; ++i) {
        // Left hand
        rawData.push_back(generateMeasurementValue(handBase, deviation, i == 2, i == 4));
    }
    for (int i = 0; i < 6; ++i) {
        // Right hand
        rawData.push_back(generateMeasurementValue(handBase, deviation, i == 2, i == 4));
    }
    for (int i = 0; i < 6; ++i) {
        // Left foot
        rawData.push_back(generateMeasurementValue(footBase, deviation, i == 3, i == 5));
    }
    for (int i = 0; i < 6; ++i) {
        // Right foot
        rawData.push_back(generateMeasurementValue(footBase, deviation, i == 3, i == 5));
    }

    return rawData;
}

// Retrieve the labels for each Ryodoraku measurement point
std::vector<std::string> DataCollector::getMeasurementLabels() const {
    return measurementLabels;
}
