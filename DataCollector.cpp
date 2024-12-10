#include "DataCollector.h"
#include <cmath>
#include <ctime>

DataCollector::DataCollector() {
    measurementLabels = {
        "H1 (Left)", "H2 (Left)", "H3 (Left)", "H4 (Left)", "H5 (Left)", "H6 (Left)",
        "H1 (Right)", "H2 (Right)", "H3 (Right)", "H4 (Right)", "H5 (Right)", "H6 (Right)",
        "F1 (Left)", "F2 (Left)", "F3 (Left)", "F4 (Left)", "F5 (Left)", "F6 (Left)",
        "F1 (Right)", "F2 (Right)", "F3 (Right)", "F4 (Right)", "F5 (Right)", "F6 (Right)"
    };
    generator.seed(static_cast<unsigned>(std::time(nullptr)));
}

float DataCollector::generateMeasurementValue(float base, float deviation, bool isExcited, bool isInhibited) {
    if (isExcited) base += 20.0f;
    if (isInhibited) base -= 20.0f;

    std::normal_distribution<float> distribution(base, deviation);
    float value = distribution(generator);
    return std::fmax(10.0f, std::fmin(150.0f, value));
}

std::vector<float> DataCollector::collectData() {
    std::vector<float> rawData;

    float handBase = 50.0f;
    float footBase = 70.0f;
    float deviation = 10.0f;

    // Left hand points
    for (int i = 0; i < 6; ++i) {
        rawData.push_back(generateMeasurementValue(handBase, deviation, i==2, i==4));
    }
    // Right hand points
    for (int i = 0; i < 6; ++i) {
        rawData.push_back(generateMeasurementValue(handBase, deviation, i==2, i==4));
    }
    // Left foot points
    for (int i = 0; i < 6; ++i) {
        rawData.push_back(generateMeasurementValue(footBase, deviation, i==3, i==5));
    }
    // Right foot points
    for (int i = 0; i < 6; ++i) {
        rawData.push_back(generateMeasurementValue(footBase, deviation, i==3, i==5));
    }

    return rawData;
}

std::vector<std::string> DataCollector::getMeasurementLabels() const {
    return measurementLabels;
}
