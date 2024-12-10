#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>
#include <string>
#include "Metric.h"

class DataProcessor {
private:
    float calculateAverage(const std::vector<float>& data);
    std::vector<std::pair<std::string, std::string>> analyzeSymmetry(
        const std::vector<float>& data, const std::vector<std::string>& labels);
    std::vector<Metric> classifyMetrics(const std::vector<float>& data, float average, const std::vector<std::string>& labels);

public:
    DataProcessor();

    // Process raw data and labels into metrics
    std::vector<Metric> processRawData(const std::vector<float>& rawData, const std::vector<std::string>& labels);
};

#endif // DATAPROCESSOR_H
