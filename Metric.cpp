#include "Metric.h"

Metric::Metric(const std::string& name, float value, const std::string& status, const std::string& description,
               float minThreshold, float maxThreshold)
    : metricName(name), value(value), status(status), description(description),
      minThreshold(minThreshold), maxThreshold(maxThreshold) {}

std::string Metric::getName() const {
    return metricName;
}

float Metric::getValue() const {
    return value;
}

std::string Metric::getStatus() const {
    return status;
}

std::string Metric::getDescription() const {
    return description;
}

float Metric::getMinThreshold() const {
    return minThreshold;
}

float Metric::getMaxThreshold() const {
    return maxThreshold;
}

void Metric::setValue(float newValue) {
    value = newValue;
}

void Metric::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Metric::setThresholds(float minT, float maxT) {
    minThreshold = minT;
    maxThreshold = maxT;
}

bool Metric::isWithinRange() const {
    return value >= minThreshold && value <= maxThreshold;
}
