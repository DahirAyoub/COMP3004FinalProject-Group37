#include "Metric.h"

Metric::Metric(const std::string& name, float value, const std::string& status)
    : metricName(name), value(value), status(status) {}

std::string Metric::getName() const {
    return metricName;
}

float Metric::getValue() const {
    return value;
}

std::string Metric::getStatus() const {
    return status;
}