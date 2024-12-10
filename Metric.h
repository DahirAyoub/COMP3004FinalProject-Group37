#ifndef METRIC_H
#define METRIC_H

#include <string>

class Metric {
private:
    std::string metricName;  // From your code
    float value;
    std::string status;
    std::string description;
    float minThreshold;
    float maxThreshold;

public:
    // Combine constructors: your constructor already covers all parameters.
    Metric(const std::string& name, float value, const std::string& status, const std::string& description,
           float minThreshold = 0.0f, float maxThreshold = 200.0f);

    std::string getName() const;
    float getValue() const;
    std::string getStatus() const;
    std::string getDescription() const;
    float getMinThreshold() const;
    float getMaxThreshold() const;

    void setValue(float newValue);
    void setStatus(const std::string& newStatus); // collaborator also had setStatus(), you have it too
    void setThresholds(float minThreshold, float maxThreshold);
    bool isWithinRange() const;
};

#endif // METRIC_H
