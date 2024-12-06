#include "HealthData.h"

HealthData::HealthData(const std::string& timestamp, const std::vector<Metric>& metrics)
    : timestamp(timestamp), metrics(metrics) {}

std::string HealthData::getTimestamp() const {
    return timestamp;
}

std::vector<Metric> HealthData::getMetrics() const {
    return metrics;
}