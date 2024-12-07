#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
#include "User.h"
#include "BatteryManager.h"
#include "DataCollector.h"
#include "DataProcessor.h"
#include "MetricsVisualizer.h"
#include "HistoricalDataManager.h"

class Device {
private:
    std::string deviceID;
    float batteryLevel;
    std::string status;

    bool skinContact;
    bool measurementDone;

    BatteryManager* batteryManager;
    DataCollector* dataCollector;
    DataProcessor* dataProcessor;
    MetricsVisualizer* visualizer;
    HistoricalDataManager* dataManager;

    std::vector<User> userProfiles;
    int nextUserID;

public:
    Device(const std::string& id);

    void createUserProfile(const std::string& name, float height, float weight, const std::string& dob);
    void updateUserProfile(int userID, const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void deleteUserProfile(int userID);
    std::vector<User> getAllProfiles() const;

    void applyToSkin();
    void liftOffSkin();

    void startMeasurement();
    void startDataCollection();
    bool checkSkinContact();
    void depleteBattery();
    void displayMetrics();
    void storeProcessedData();
};

#endif // DEVICE_H
