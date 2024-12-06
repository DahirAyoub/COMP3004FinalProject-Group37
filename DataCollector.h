#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <vector>

class DataCollector {
private:
    std::vector<float> rawData;

public:
    std::vector<float> collectData();
};

#endif // DATACOLLECTOR_H