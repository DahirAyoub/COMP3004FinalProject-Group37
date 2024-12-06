#include "DataProcessor.h"

// Interpret the value based on thresholds (Ryodoraku ranges)
std::string DataProcessor::interpretValue(float value) const {
    if (value < 50) {
        return "Low";
    } else if (value <= 100) {
        return "Normal";
    } else {
        return "High";
    }
}

// Process raw data into metrics
std::vector<Metric> DataProcessor::processRawData(const std::vector<float>& rawData) {
    std::vector<Metric> metrics;

    // Generate metrics for each data point
    for (size_t i = 0; i < rawData.size(); ++i) {
        float value = rawData[i];
        std::string status = interpretValue(value);

        // Create a metric object with a name, value, and status
        metrics.emplace_back("Measurement " + std::to_string(i + 1), value, status);
    }

    return metrics;
}