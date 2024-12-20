#ifndef METRIC_H
#define METRIC_H

#include <string>

class Metric {
private:
    std::string metricName;
    float value;
    std::string status;
    std::string description;
    float minThreshold;
    float maxThreshold;

public:
    // Default constructor to allow default-initialization
    Metric();

    // Parameterized constructor
    Metric(const std::string& name, float value, const std::string& status, const std::string& description,
           float minThreshold = 0.0f, float maxThreshold = 200.0f);

    std::string getName() const;
    float getValue() const;
    std::string getStatus() const;
    std::string getDescription() const;
    float getMinThreshold() const;
    float getMaxThreshold() const;

    void setValue(float newValue);
    void setStatus(const std::string& newStatus);
    void setThresholds(float minThreshold, float maxThreshold);
    bool isWithinRange() const;
};

#endif // METRIC_H
