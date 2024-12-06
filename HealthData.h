#ifndef HEALTHDATA_H
#define HEALTHDATA_H

#include <vector>
#include <string>
#include "Metric.h"

class HealthData {
private:
    std::string timestamp;
    std::vector<Metric> metrics;

public:
    HealthData(const std::string& timestamp, const std::vector<Metric>& metrics);
    std::string getTimestamp() const;
    std::vector<Metric> getMetrics() const;
};

#endif // HEALTHDATA_H