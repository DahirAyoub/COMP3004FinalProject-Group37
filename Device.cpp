#include "Device.h"
#include "Metric.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random>

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
      dataManager(new HistoricalDataManager())
{
    // Initialize default metrics once the device is created
    initializeDefaultMetrics();
}

static std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::tm tmStruct = *std::localtime(&timeT);

    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

float Device::normalizeValue(float rawValue, float rawMin, float rawMax, float targetMin, float targetMax) {
    return ((rawValue - rawMin) / (rawMax - rawMin)) * (targetMax - targetMin) + targetMin;
}

void Device::depleteBatteryDuringOperation(float amount) {
    batteryManager->depleteBattery(amount);
    if (getBatteryLevel() <= 10.0f) {
        showLowPowerWarning();
    }
}

void Device::showLowPowerWarning() const {
    std::cerr << "Warning: Low battery! Please recharge the device.\n";
}

bool Device::checkSkinContact() const {
    return skinContact;
}

void Device::initializeDefaultMetrics() {
    // Assign default values and thresholds for each region
    bodyRegionMetrics = {
        {"Head", Metric("Head", 100.0f, "Normal", "Conductivity of the head region.", 80.0f, 120.0f)},
        {"Neck", Metric("Neck", 100.0f, "Normal", "Conductivity of the neck region.", 80.0f, 120.0f)},
        {"Left Shoulder", Metric("Left Shoulder", 100.0f, "Normal", "Conductivity of the left shoulder.", 80.0f, 120.0f)},
        {"Right Shoulder", Metric("Right Shoulder", 100.0f, "Normal", "Conductivity of the right shoulder.", 80.0f, 120.0f)},
        {"Heart", Metric("Heart", 100.0f, "Normal", "Conductivity of the heart region.", 80.0f, 120.0f)},
        {"Lungs", Metric("Lungs", 100.0f, "Normal", "Conductivity of the lungs.", 80.0f, 120.0f)},
        {"Stomach", Metric("Stomach", 100.0f, "Normal", "Conductivity of the stomach region.", 80.0f, 120.0f)},
        {"Left Kidney", Metric("Left Kidney", 100.0f, "Normal", "Conductivity of the left kidney.", 80.0f, 120.0f)},
        {"Right Kidney", Metric("Right Kidney", 100.0f, "Normal", "Conductivity of the right kidney.", 80.0f, 120.0f)},
        {"Left Foot", Metric("Left Foot", 100.0f, "Normal", "Conductivity of the left foot.", 80.0f, 120.0f)},
        {"Right Foot", Metric("Right Foot", 100.0f, "Normal", "Conductivity of the right foot.", 80.0f, 120.0f)}
    };
}

// From your version of user management:
bool Device::registerAccount(const std::string& username, const std::string& password) {
    if (credentials.count(username)) {
        std::cout << "Error: Username already exists.\n";
        return false;
    }
    credentials[username] = password;
    std::cout << "Account registered successfully.\n";
    return true;
}

bool Device::authenticate(const std::string& username, const std::string& password) {
    // Unified authentication approach
    if (credentials.find(username) == credentials.end() || credentials[username] != password) {
        std::cout << "Authentication failed: Invalid username or password.\n";
        return false;
    }
    currentLoggedInUser = username;
    std::cout << "Authentication successful for user: " << username << "\n";
    return true;
}

bool Device::login(const std::string& username, const std::string& password) {
    return authenticate(username, password);
}

void Device::logout() {
    if (!currentLoggedInUser.empty()) {
        std::cout << "User " << currentLoggedInUser << " logged out.\n";
        currentLoggedInUser.clear();
    } else {
        std::cout << "No user is currently logged in.\n";
    }
}

// Create a new user profile (your simpler version)
void Device::createUserProfile(const std::string& name, float height, float weight, const std::string& dob) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in.\n";
        return;
    }
    accounts[currentLoggedInUser] = User(nextUserID++, name, height, weight, dob);
    std::cout << "Profile created successfully for " << currentLoggedInUser << ".\n";
}

// Create a new user profile (collaborator’s version with username/password)
bool Device::createUserProfile(const std::string& name, float height, float weight, const std::string& dob,
                               const std::string& username, const std::string& password) {
    // Check max profiles
    if ((int)accounts.size() >= maxProfiles) {
        std::cout << "Error: Maximum number of profiles (" << maxProfiles << ") reached.\n";
        return false;
    }

    if (credentials.find(username) != credentials.end()) {
        std::cout << "Error: Username '" << username << "' already exists.\n";
        return false;
    }

    // Register account
    if (!registerAccount(username, password)) {
        return false;
    }

    accounts[username] = User(nextUserID++, name, height, weight, dob, username, password);
    accounts[username].createProfile();
    return true;
}

// Update profile (your version)
void Device::updateUserProfile(const std::string& username, const std::string& newName, float newHeight, float newWeight, const std::string& newDob) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in.\n";
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

// Update profile (collaborator’s extended version with username/password)
void Device::updateUserProfile(int userID, const std::string& newName, float newHeight, float newWeight,
                               const std::string& newDob, const std::string& newUsername, const std::string& newPassword) {
    // Find user by ID in accounts map
    for (auto& kv : accounts) {
        User& user = kv.second;
        if (user.getUserID() == userID) {
            // Update and also update credentials if username changes
            if (newUsername != kv.first) {
                if (credentials.find(newUsername) != credentials.end()) {
                    std::cout << "Error: New username '" << newUsername << "' already exists.\n";
                    return;
                }
                credentials.erase(kv.first);
                credentials[newUsername] = newPassword;
                accounts.erase(kv.first);
                accounts[newUsername] = User(userID, newName, newHeight, newWeight, newDob, newUsername, newPassword);
                std::cout << "Profile updated successfully with new username.\n";
                return;
            }

            // Same username, just update password and other fields
            credentials[newUsername] = newPassword;
            user.updateProfile(userID, newName, newHeight, newWeight, newDob, newUsername, newPassword);
            return;
        }
    }

    std::cout << "Error: User with ID " << userID << " not found.\n";
}

// Delete profile
void Device::deleteUserProfile(int userID) {
    if (!isLoggedIn()) {
        std::cout << "Error: No user logged in.\n";
        return;
    }

    // Check if logged-in user's ID matches userID
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->second.getUserID() == userID) {
            // Remove from credentials
            credentials.erase(it->first);
            it->second.deleteProfile();
            accounts.erase(it);
            std::cout << "Profile deleted successfully.\n";
            logout();
            return;
        }
    }
    std::cout << "Error: User ID does not match any profile.\n";
}

std::vector<User> Device::getAllProfiles() const {
    std::vector<User> profiles;
    for (const auto& kv : accounts) {
        profiles.push_back(kv.second);
    }
    return profiles;
}

// Get user profile by username
const User* Device::getUserProfile(const std::string& username) const {
    auto it = accounts.find(username);
    if (it != accounts.end()) {
        return &it->second;
    }
    return nullptr;
}

std::string Device::getLoggedInUser() const {
    return currentLoggedInUser;
}

bool Device::isLoggedIn() const {
    return !currentLoggedInUser.empty();
}

// Apply and lift device from skin
void Device::applyToSkin() {
    if (skinContact) {
        std::cout << "Device is already on the skin.\n";
        return;
    }
    skinContact = true;
    measurementDone = false;
    std::cout << "Device applied to skin.\n";
}

void Device::liftOffSkin() {
    if (!skinContact) {
        std::cout << "Device is not on the skin.\n";
        return;
    }
    skinContact = false;
    std::cout << "Device lifted off skin.\n";
}

float Device::getBatteryLevel() const {
    return batteryManager->checkBatteryLevel();
}

// Start a measurement (enhanced)
void Device::startMeasurement() {
    if (!checkSkinContact()) {
        std::cerr << "Error: Device must be applied to skin before starting measurement.\n";
        return;
    }

    std::cout << "Starting measurement procedure...\n";
    std::cout << "Current battery level: " << getBatteryLevel() << "%\n";

    if (getBatteryLevel() <= 5.0f) {
        std::cerr << "Error: Battery too low.\n";
        return;
    }

    depleteBatteryDuringOperation(2.0f);

    std::cout << "Collecting and processing metrics...\n";

    std::vector<Metric> metrics;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0f, 200.0f);

    for (auto &pair : bodyRegionMetrics) {
        Metric &metric = pair.second;
        float randomValue = static_cast<float>(dist(gen));
        float normalizedValue = normalizeValue(randomValue, 0.0f, 200.0f, metric.getMinThreshold(), metric.getMaxThreshold());
        metric.setValue(normalizedValue);

        if (normalizedValue < metric.getMinThreshold())
            metric.setStatus("Low");
        else if (normalizedValue > metric.getMaxThreshold())
            metric.setStatus("High");
        else
            metric.setStatus("Normal");

        metrics.push_back(metric);
    }

    storeProcessedData(metrics);

    std::cout << "Battery level after measurement: " << getBatteryLevel() << "%\n";

    for (const auto &metric : metrics) {
        std::cout << metric.getName() << ": " << metric.getValue() << " μA (" << metric.getStatus() << ")\n";
    }

    std::cout << "Measurement concluded.\n";
}
void Device::startDataCollection() {
    if (!checkSkinContact()) {
        std::cout << "Error: Device must be applied to skin to collect data.\n";
        return;
    }

    std::cout << "Data collection started...\n";
    std::vector<float> rawData = dataCollector->collectData();
    auto labels = dataCollector->getMeasurementLabels();

    std::cout << "Data collected from " << rawData.size() << " body points:\n";
    for (size_t i = 0; i < rawData.size() && i < labels.size(); ++i) {
        std::cout << labels[i] << " - Value: " << rawData[i] << "\n";
    }

    std::cout << "Data collection completed.\n";

    // Could process and store data if needed
}

void Device::depleteBattery() {
    batteryManager->depleteBattery(5.0f);
}

void Device::displayMetrics() {
    // Placeholder
}

void Device::storeProcessedData(const std::vector<Metric>& metrics) {
    if (!isLoggedIn()) {
        std::cerr << "Error: No user logged in.\n";
        return;
    }

    // Generate timestamp for the current record
    std::string timestamp = getCurrentTimestamp();

    // Create a new HealthData record
    HealthData record(timestamp, metrics);

    // Retrieve the current user
    User &user = accounts[currentLoggedInUser];

    // Store the data using DataManager
    dataManager->storeData(user.getUserID(), record);

    std::cout << "Processed data stored for user: " << currentLoggedInUser << "\n";
}

std::vector<HealthData> Device::getAllUserData() const {
    if (!isLoggedIn()) {
        std::cerr << "Error: No user logged in.\n";
        return {};
    }

    // Retrieve the current user
    const User &user = accounts.at(currentLoggedInUser);

    // Retrieve all data for the user from DataManager
    return dataManager->retrieveData(user.getUserID());
}


// Collect measurement data
std::vector<float> Device::collectMeasurementData() {
    return dataCollector->collectData();
}

// Get measurement labels
std::vector<std::string> Device::getMeasurementLabels() const {
    return dataCollector->getMeasurementLabels();
}

std::vector<Metric> Device::getBodyRegionMetrics() const {
    std::vector<Metric> metrics;
    for (const auto& pair : bodyRegionMetrics) {
        metrics.push_back(pair.second);
    }
    return metrics;
}

std::string Device::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(buffer);
}
