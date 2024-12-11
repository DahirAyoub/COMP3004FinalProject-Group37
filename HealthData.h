#ifndef HEALTHDATA_H
#define HEALTHDATA_H

#include <vector>
#include <string>
#include "Metric.h"

class HealthData {
private:
    std::string timestamp;       // Timestamp when the data was collected
    std::vector<Metric> metrics; // List of metrics
    std::string userNotes;       // Optional: User notes or additional context

public:
    HealthData(const std::string& timestamp, const std::vector<Metric>& metrics, const std::string& notes = "");

    std::string getTimestamp() const;
    std::vector<Metric> getMetrics() const;
    std::string getUserNotes() const;

    void setUserNotes(const std::string& notes);
    void addMetric(const Metric& metric);
};

#endif // HEALTHDATA_H
