#ifndef HISTORICALDATAMANAGER_H
#define HISTORICALDATAMANAGER_H

#include <vector>
#include "HealthData.h"

class HistoricalDataManager {
private:
    std::vector<HealthData> historicalRecords;

public:
    void storeData(const HealthData& data);
    std::vector<HealthData> retrieveData(int userID);
};

#endif // HISTORICALDATAMANAGER_H