#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>
#include <string>
#include "Metric.h"

class DataProcessor {
private:
    std::vector<Metric> processedMetrics;

    // Interpret the value based on defined thresholds
    std::string interpretValue(float value) const;

public:
    std::vector<Metric> processRawData(const std::vector<float>& rawData);
};

#endif // DATAPROCESSOR_H