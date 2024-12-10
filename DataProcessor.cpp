#include "DataProcessor.h"
#include <numeric>
#include <cmath>

// Calculate average
float DataProcessor::calculateAverage(const std::vector<float>& data) {
    float sum = std::accumulate(data.begin(), data.end(), 0.0f);
    return sum / static_cast<float>(data.size());
}

std::vector<std::pair<std::string, std::string>> DataProcessor::analyzeSymmetry(
    const std::vector<float>& data, const std::vector<std::string>& labels) {
    std::vector<std::pair<std::string, std::string>> imbalances;

    for (size_t i = 0; i < data.size() / 2; ++i) {
        float diff = std::fabs(data[i] - data[i + data.size() / 2]);
        if (diff > 10.0f) {
            imbalances.emplace_back(labels[i], labels[i + data.size() / 2]);
        }
    }

    return imbalances;
}

std::vector<Metric> DataProcessor::classifyMetrics(const std::vector<float>& data, float average, const std::vector<std::string>& labels) {
    std::vector<Metric> metrics;

    for (size_t i = 0; i < data.size(); ++i) {
        std::string status = "Normal";
        if (data[i] > average + 20.0f) {
            status = "Excitation";
        } else if (data[i] < average - 20.0f) {
            status = "Inhibition";
        }

        // Use a generic description, or could be extended:
        std::string description = "Ryodoraku point conductivity measurement.";
        metrics.emplace_back(labels[i], data[i], status, description, 80.0f, 120.0f);
    }

    return metrics;
}

DataProcessor::DataProcessor() = default;

std::vector<Metric> DataProcessor::processRawData(const std::vector<float>& rawData, const std::vector<std::string>& labels) {
    float average = calculateAverage(rawData);
    std::vector<Metric> metrics = classifyMetrics(rawData, average, labels);
    auto imbalances = analyzeSymmetry(rawData, labels);

    for (const auto& pair : imbalances) {
        for (auto& metric : metrics) {
            if (metric.getName() == pair.first || metric.getName() == pair.second) {
                metric.setStatus("Imbalance Detected");
            }
        }
    }

    return metrics;
}
