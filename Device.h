#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
#include <map>
#include "User.h"
#include "BatteryManager.h"
#include "DataCollector.h"
#include "DataProcessor.h"
#include "MetricsVisualizer.h"
#include "HistoricalDataManager.h"
#include "Metric.h" // Ensure we have Metric included if not already

class Device {
private:
    std::string deviceID;
    float batteryLevel;
    std::string status;

    std::string currentLoggedInUser; // Current logged-in username
    int nextUserID;

    bool skinContact;
    bool measurementDone;

    BatteryManager* batteryManager;
    DataCollector* dataCollector;
    DataProcessor* dataProcessor;
    MetricsVisualizer* visualizer;
    HistoricalDataManager* dataManager;

    std::map<std::string, std::string> credentials; // Username-password map
    std::map<std::string, User> accounts;           // Username-user profile map

public:
    Device(const std::string& id);

    // User management
    void createUserProfile(const std::string& name, float height, float weight, const std::string& dob);
    void updateUserProfile(const std::string& username, const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void deleteUserProfile(int userID);
    std::vector<User> getAllProfiles() const;

    // Authentication
    bool registerAccount(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    std::string getLoggedInUser() const; // Get currently logged-in user
    void logout();

    // Skin contact methods
    void applyToSkin();
    void liftOffSkin();

    // Measurement-related methods
    void startMeasurement();
    void startDataCollection();
    bool checkSkinContact();
    void depleteBattery();
    void displayMetrics();

    // Store processed data for the current user
    void storeProcessedData(const std::vector<Metric>& metrics);

    // Helpers
    bool isLoggedIn() const;
};

#endif // DEVICE_H
