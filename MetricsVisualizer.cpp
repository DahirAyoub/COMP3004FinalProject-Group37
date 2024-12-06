#include "MetricsVisualizer.h"
#include <iostream>

MetricsVisualizer::MetricsVisualizer(const std::string& type)
    : visualizationType(type) {}

void MetricsVisualizer::displayMetrics(const std::vector<Metric>& metrics) {
    for (const auto& metric : metrics) {
        std::cout << "Metric: " << metric.getName()
                  << ", Value: " << metric.getValue()
                  << ", Range: " << metric.getRange() << std::endl;
    }
}