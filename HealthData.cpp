#include "HealthData.h"

HealthData::HealthData(const std::string& timestamp, const std::vector<Metric>& metrics, const std::string& notes)
    : timestamp(timestamp), metrics(metrics), userNotes(notes) {}

std::string HealthData::getTimestamp() const {
    return timestamp;
}

std::vector<Metric> HealthData::getMetrics() const {
    return metrics;
}

std::string HealthData::getUserNotes() const {
    return userNotes;
}

void HealthData::setUserNotes(const std::string& notes) {
    userNotes = notes;
}

void HealthData::addMetric(const Metric& metric) {
    metrics.push_back(metric);
}
