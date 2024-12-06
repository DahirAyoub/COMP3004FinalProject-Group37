#ifndef METRICSVISUALIZER_H
#define METRICSVISUALIZER_H

#include <vector>
#include "Metric.h"

class MetricsVisualizer {
private:
    std::string visualizationType;

public:
    MetricsVisualizer(const std::string& type);
    void displayMetrics(const std::vector<Metric>& metrics);
};

#endif // METRICSVISUALIZER_H