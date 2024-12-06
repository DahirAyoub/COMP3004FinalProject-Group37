#include "DataProcessor.h"

std::vector<Metric> DataProcessor::processRawData(const std::vector<float>& rawData) {
    std::vector<Metric> metrics;
    for (float value : rawData) {
        metrics.emplace_back("MetricName", value, "Normal"); // Example metrics
    }
    return metrics;
}