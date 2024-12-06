#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <vector>
#include <string>

class DataCollector {
private:
    std::vector<float> rawData;
    std::vector<std::string> measurementLabels;

public:
    DataCollector();

    std::vector<float> collectData();
    std::vector<std::string> getMeasurementLabels() const;
};

#endif // DATACOLLECTOR_H