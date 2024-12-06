#include "Device.h"
#include <iostream>
#include <algorithm>

Device::Device(const std::string& id)
    : deviceID(id), batteryLevel(100.0f), status("Idle"), skinContact(false), measurementDone(false),
      batteryManager(new BatteryManager(100.0f)),
      dataCollector(new DataCollector()),
      dataProcessor(new DataProcessor()),
      visualizer(new MetricsVisualizer("Chart")),
      dataManager(new HistoricalDataManager()) {}

// Create a new user profile on this device
void Device::createUserProfile(int userID, const std::string& name, int age) {
    if (userProfiles.size() >= 5) {
        std::cout << "Error: Maximum number of profiles (5) reached.\n";
        return;
    }

    // Check if userID already exists
    for (const auto& user : userProfiles) {
        if (user.getUserID() == userID) {
            std::cout << "Error: A user with ID " << userID << " already exists.\n";
            return;
        }
    }

    // Add the new user and call createProfile()
    User newUser(userID, name, age);
    userProfiles.push_back(newUser);
    userProfiles.back().createProfile();
}

// Update an existing user profile
void Device::updateUserProfile(int userID, const std::string& newName, int newAge) {
    for (auto& user : userProfiles) {
        if (user.getUserID() == userID) {
            user.updateProfile(userID, newName, newAge);
            return;
        }
    }
    std::cout << "Error: User with ID " << userID << " not found.\n";
}

// Delete a user profile
void Device::deleteUserProfile(int userID) {
    for (auto it = userProfiles.begin(); it != userProfiles.end(); ++it) {
        if (it->getUserID() == userID) {
            it->deleteProfile(userID);
            userProfiles.erase(it);
            return;
        }
    }
    std::cout << "Error: User with ID " << userID << " not found.\n";
}

// Skin contact: Apply device to skin
void Device::applyToSkin() {
    if (skinContact) {
        std::cout << "Device is already on the skin.\n";
        return;
    }
    skinContact = true;
    measurementDone = false; // Reset measurement flag
    std::cout << "Device applied to skin.\n";
}

// Skin contact: Lift device off the skin
void Device::liftOffSkin() {
    if (!skinContact) {
        std::cout << "Device is not on the skin.\n";
        return;
    }
    skinContact = false;
    std::cout << "Device lifted off the skin.\n";
}

// Check if the device is in contact with the skin
bool Device::checkSkinContact() {
    return skinContact;
}

// Start a measurement (enforce skin contact rules)
void Device::startMeasurement() {
    if (!skinContact) {
        std::cout << "Error: Apply the device to the skin before starting measurement.\n";
        return;
    }

    if (measurementDone) {
        std::cout << "Error: You must lift the device off the skin before starting a new measurement.\n";
        return;
    }

    std::cout << "Starting measurement...\n";

    // Collect data
    startDataCollection();

    // Simulate measurement complete
    measurementDone = true;
    std::cout << "Measurement complete. Lift the device off the skin to reset.\n";
}

// Data collection method
void Device::startDataCollection() {
    if (!checkSkinContact()) {
        std::cout << "Error: Device must be applied to skin to collect data.\n";
        return;
    }

    std::cout << "Data collection started...\n";

    // Use DataCollector to collect raw data
    std::vector<float> rawData = dataCollector->collectData();

    // Display collected data
    std::cout << "Data collected from 24 body points:\n";
    for (size_t i = 0; i < rawData.size(); ++i) {
        std::cout << "Point " << (i + 1) << ": " << rawData[i] << "\n";
    }

    std::cout << "Data collection completed.\n";
}

void Device::depleteBattery() {
    batteryManager->depleteBattery(5.0f);
}

void Device::displayMetrics() {
    // Placeholder for displaying previously processed metrics
}

void Device::storeProcessedData() {
    // Placeholder for storing processed metrics in historical data
}