#include "HistoricalDataManager.h"

void HistoricalDataManager::storeData(int userID, const HealthData& data) {
    userHistory[userID].push_back(data);
}

std::vector<HealthData> HistoricalDataManager::retrieveData(int userID) const {
    auto it = userHistory.find(userID);
    if (it != userHistory.end()) {
        return it->second;
    }
    return {};
}
