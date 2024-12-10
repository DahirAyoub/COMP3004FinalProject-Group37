#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <vector>
#include <string>
#include <random>

class DataCollector {
private:
    std::vector<std::string> measurementLabels;
    std::default_random_engine generator;

    float generateMeasurementValue(float base, float deviation, bool isExcited, bool isInhibited);

public:
    DataCollector();

    std::vector<float> collectData();
    std::vector<std::string> getMeasurementLabels() const;
};

#endif // DATACOLLECTOR_H
