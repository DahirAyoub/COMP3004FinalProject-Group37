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

    bool skinContact;          // Indicates if the device is on the skin
    bool measurementDone;      // Ensures lifting off between measurements

    BatteryManager* batteryManager;
    DataCollector* dataCollector;
    DataProcessor* dataProcessor;
    MetricsVisualizer* visualizer;
    HistoricalDataManager* dataManager;

    std::vector<User> userProfiles; // Store multiple user profiles (up to 5)
    int nextUserID;                 // Tracks the next available user ID

public:
    Device(const std::string& id);

    // User management methods
    void createUserProfile(const std::string& name, int age, float height, float weight, const std::string& dob);
    void updateUserProfile(int userID, const std::string& newName, int newAge, float newHeight, float newWeight, const std::string& newDob);
    void deleteUserProfile(int userID);
    std::vector<User> getAllProfiles() const;

    // Skin contact methods
    void applyToSkin();
    void liftOffSkin();

    // Measurement-related methods
    void startMeasurement();
    void startDataCollection();
    bool checkSkinContact();
    void depleteBattery();
    void displayMetrics();
    void storeProcessedData();
};

#endif // DEVICE_H
