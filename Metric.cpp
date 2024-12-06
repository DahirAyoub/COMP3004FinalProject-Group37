#include "Metric.h"

Metric::Metric(const std::string& name, float value, const std::string& range)
    : metricName(name), value(value), range(range) {}

std::string Metric::getName() const {
    return metricName;
}

float Metric::getValue() const {
    return value;
}

std::string Metric::getRange() const {
    return range;
}