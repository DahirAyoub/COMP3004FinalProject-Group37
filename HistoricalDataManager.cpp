#include "HistoricalDataManager.h"

void HistoricalDataManager::storeData(const HealthData& data) {
    historicalRecords.push_back(data);
}

std::vector<HealthData> HistoricalDataManager::retrieveData(int userID) {
    // Retrieve all historical data for the given user (example implementation)
    return historicalRecords;
}