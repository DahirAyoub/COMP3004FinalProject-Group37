#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
#include <map>
#include "User.h"
#include "Metric.h"
#include "BatteryManager.h"
#include "DataCollector.h"
#include "DataProcessor.h"
#include "MetricsVisualizer.h"
#include "HistoricalDataManager.h"
#include "HealthData.h"

class Device {
private:
    std::string deviceID;
    float batteryLevel;
    std::string status;

    std::string currentLoggedInUser;
    int nextUserID;

    bool skinContact;
    bool measurementDone;

    BatteryManager* batteryManager;
    DataCollector* dataCollector;
    DataProcessor* dataProcessor;
    MetricsVisualizer* visualizer;
    HistoricalDataManager* dataManager;

    // Authentication and profiles
    std::map<std::string, std::string> credentials;
    std::map<std::string, User> accounts;

    static const int maxProfiles = 5;

    // Body region metrics (last measured)
    std::map<std::string, Metric> bodyRegionMetrics;

    void initializeDefaultMetrics();
    static float normalizeValue(float rawValue, float rawMin = 0.0f, float rawMax = 200.0f,
                                float targetMin = 80.0f, float targetMax = 120.0f);
    void depleteBatteryDuringOperation(float amount);
    void showLowPowerWarning() const;

    std::string getCurrentTimestamp() const;

public:
    Device(const std::string& id);

    // User management
    bool registerAccount(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    bool authenticate(const std::string& username, const std::string& password);
    void logout();
    std::string getLoggedInUser() const;

    void createUserProfile(const std::string& name, float height, float weight, const std::string& dob);
    bool createUserProfile(const std::string& name, float height, float weight, const std::string& dob,
                           const std::string& username, const std::string& password);
    void updateUserProfile(const std::string& username, const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void updateUserProfile(int userID, const std::string& newName, float newHeight, float newWeight,
                           const std::string& newDob, const std::string& newUsername, const std::string& newPassword);
    void deleteUserProfile(int userID);
    std::vector<User> getAllProfiles() const;
    const User* getUserProfile(const std::string& username) const;

    bool isLoggedIn() const;

    // Skin contact
    void applyToSkin();
    void liftOffSkin();
    bool checkSkinContact() const;

    // Measurement
    void startMeasurement();
    void startDataCollection();
    void depleteBattery();
    void displayMetrics();
    void storeProcessedData(const std::vector<Metric>& metrics);

    float getBatteryLevel() const;

    // Historical data
    std::vector<HealthData> getAllUserData() const;

    // Data collection
    std::vector<float> collectMeasurementData();
    std::vector<std::string> getMeasurementLabels() const;

    // Get metrics
    std::vector<Metric> getBodyRegionMetrics() const;

};

#endif // DEVICE_H