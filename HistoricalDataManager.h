#ifndef HISTORICALDATAMANAGER_H
#define HISTORICALDATAMANAGER_H

#include <vector>
#include <map>
#include "HealthData.h"

class HistoricalDataManager {
private:
    // Map of userID to a vector of HealthData records
    std::map<int, std::vector<HealthData>> userHistory;

public:
    // Store HealthData for a specific user
    void storeData(int userID, const HealthData& data);

    // Retrieve all HealthData for a given userID
    std::vector<HealthData> retrieveData(int userID) const;
};

#endif // HISTORICALDATAMANAGER_H
