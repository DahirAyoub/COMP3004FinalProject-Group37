#ifndef METRIC_H
#define METRIC_H

#include <string>

class Metric {
private:
    std::string metricName;
    float value;
    std::string range;

public:
    Metric(const std::string& name, float value, const std::string& range);
    std::string getName() const;
    float getValue() const;
    std::string getRange() const;
};

#endif // METRIC_H