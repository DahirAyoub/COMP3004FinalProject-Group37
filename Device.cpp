#include "Device.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

// Constructor
Device::Device(const std::string& id)
    : deviceID(id),
      batteryLevel(100.0f),
      status("Idle"),
      currentLoggedInUser(""),
      nextUserID(1),
      skinContact(false),
      measurementDone(false),
      batteryManager(new BatteryManager(100.0f)),
      dataCollector(new DataCollector()),
      dataProcessor(new DataProcessor()),
      visualizer(new MetricsVisualizer("Chart")),
      dataManager(new HistoricalDataManager()) {}

static std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::tm tmStruct = *std::localtime(&timeT);

    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Authentication
bool Device::registerAccount(const std::string& username, const std::string& password) {
    if (credentials.count(username)) {
        std::cout << "Error: Username already exists.\n";
        return false;
    }
    credentials[username] = password;
    std::cout << "Account registered successfully.\n";
    return true;
}

bool Device::login(const std::string& username, const std::string& password) {
    if (credentials.find(username) == credentials.end() || credentials[username] != password) {
        std::cout << "Error: Invalid username or password.\n";
        return false;
    }
    currentLoggedInUser = username;
    std::cout << "Logged in successfully as " << username << ".\n";

    // Load or create user profile
    if (accounts.find(username) == accounts.end()) {
        accounts[username] = User(nextUserID++, username, 0.0f, 0.0f, "");
    }
    return true;
}

void Device::logout() {
    if (currentLoggedInUser.empty()) {
        std::cout << "No user is currently logged in.\n";
        return;
    }
    std::cout << "User " << currentLoggedInUser << " logged out.\n";
    currentLoggedInUser.clear();
}

// Create a new user profile
void Device::createUserProfile(const std::string& name, float height, float weight, const std::string& dob) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in. Please log in to create a profile.\n";
        return;
    }
    accounts[currentLoggedInUser] = User(nextUserID++, name, height, weight, dob);
    std::cout << "Profile created successfully for " << currentLoggedInUser << ".\n";
}

// Update an existing user profile
void Device::updateUserProfile(const std::string& username, const std::string& newName, float newHeight, float newWeight, const std::string& newDob) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in. Please log in to update a profile.\n";
        return;
    }
    if (accounts.find(username) == accounts.end()) {
        std::cout << "Error: User profile not found.\n";
        return;
    }
    User& user = accounts[username];
    user.updateProfile(newName, newHeight, newWeight, newDob);
    std::cout << "Profile updated successfully for " << username << ".\n";
}

// Delete a user profile
void Device::deleteUserProfile(int userID) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in. Please log in to delete a profile.\n";
        return;
    }
    auto& user = accounts[currentLoggedInUser];
    if (user.getUserID() != userID) {
        std::cout << "Error: User ID does not match the logged-in user.\n";
        return;
    }
    accounts.erase(currentLoggedInUser);
    std::cout << "Profile deleted successfully for " << currentLoggedInUser << ".\n";
    logout();
}

// Get all profiles (for debugging purposes)
std::vector<User> Device::getAllProfiles() const {
    std::vector<User> profiles;
    for (const auto& account : accounts) {
        profiles.push_back(account.second);
    }
    return profiles;
}

// Get the logged-in username
std::string Device::getLoggedInUser() const {
    return currentLoggedInUser;
}

// Helpers
bool Device::isLoggedIn() const {
    return !currentLoggedInUser.empty();
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
    if (!checkSkinContact()) {
        std::cout << "Error: Apply the device to the skin before starting measurement.\n";
        return;
    }

    std::cout << "Starting measurement...\n";

    // Collect raw data
    std::vector<float> rawData = dataCollector->collectData();

    // Process raw data into metrics
    std::vector<Metric> metrics = dataProcessor->processRawData(rawData);

    // Display metrics
    std::cout << "Processed Health Metrics:\n";
    for (const auto& metric : metrics) {
        std::cout << metric.getName() << " - Value: " << metric.getValue()
                  << " - Status: " << metric.getStatus() << "\n";
    }

    std::cout << "Measurement complete.\n";
}

// Data collection method
void Device::startDataCollection() {
    if (!checkSkinContact()) {
        std::cout << "Error: Device must be applied to skin to collect data.\n";
        return;
    }

    std::cout << "Data collection started...\n";

    // Collect raw data and labels
    std::vector<float> rawData = dataCollector->collectData();
    std::vector<std::string> labels = dataCollector->getMeasurementLabels();

    // Display collected data with labels
    std::cout << "Data collected from 24 body points:\n";
    for (size_t i = 0; i < rawData.size(); ++i) {
        std::cout << labels[i] << " - Value: " << rawData[i] << "\n";
    }

    std::cout << "Data collection completed.\n";
}

void Device::depleteBattery() {
    batteryManager->depleteBattery(5.0f);
}

void Device::displayMetrics() {
    // Placeholder for displaying previously processed metrics
}

void Device::storeProcessedData(const std::vector<Metric>& metrics) {
    if (!isLoggedIn()) {
        std::cout << "Error: Cannot store data, no user logged in.\n";
        return;
    }

    User &user = accounts[currentLoggedInUser];
    int userID = user.getUserID();

    std::string timestamp = getCurrentTimestamp();
    HealthData record(timestamp, metrics);

    // Now we store data with the userID
    dataManager->storeData(userID, record);

    std::cout << "Processed data stored for user: " << currentLoggedInUser << "\n";
}
