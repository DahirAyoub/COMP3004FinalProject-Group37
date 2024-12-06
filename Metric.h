#ifndef METRIC_H
#define METRIC_H

#include <string>

class Metric {
private:
    std::string metricName; // Name of the metric
    float value;            // Measured value
    std::string status;     // Status (e.g., "Normal", "High", "Low")

public:
    Metric(const std::string& name, float value, const std::string& status);

    std::string getName() const;
    float getValue() const;
    std::string getStatus() const;
};

#endif // METRIC_H