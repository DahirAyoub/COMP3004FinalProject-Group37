#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>
#include "Metric.h"

class DataProcessor {
private:
    std::vector<Metric> processedMetrics;

public:
    std::vector<Metric> processRawData(const std::vector<float>& rawData);
};

#endif // DATAPROCESSOR_H