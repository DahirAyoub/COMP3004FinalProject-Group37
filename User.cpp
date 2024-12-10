#include "User.h"
#include "HistoricalDataManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <functional> // For std::hash

// Helper function to hash a password (from collaborator)
std::string User::hashPassword(const std::string& password) const {
    // Placeholder hash
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

// Merged Constructor: we have two constructors from user code.
// We'll keep the collaborator's full constructor and also add a simpler one if needed.
User::User() : userID(-1), name(""), height(0.0f), weight(0.0f), dob(""), username(""), passwordHash("") {}

User::User(int id, const std::string& name, float height, float weight, const std::string& dob)
    : userID(id), name(name), height(height), weight(weight), dob(dob), username(""), passwordHash("") {}

User::User(int id, const std::string& name, float height, float weight, const std::string& dob,
           const std::string& username, const std::string& password)
    : userID(id), name(name), height(height), weight(weight), dob(dob),
      username(username), passwordHash(hashPassword(password)) {}

// Create Profile
void User::createProfile() {
    std::cout << "User profile created with ID: " << userID
              << ", Name: " << name
              << ", Height: " << height << " cm"
              << ", Weight: " << weight << " kg"
              << ", DOB: " << dob
              << (username.empty() ? "" : (", Username: " + username))
              << "\n";
}

// Update Profile (without username/password from user code)
void User::updateProfile(const std::string& newName, float newHeight, float newWeight, const std::string& newDob) {
    name = newName;
    height = newHeight;
    weight = newWeight;
    dob = newDob;
    std::cout << "User profile updated: Name: " << name
              << ", Height: " << height << " cm"
              << ", Weight: " << weight << " kg"
              << ", DOB: " << dob << "\n";
}

// Update Profile (collaborator extended)
void User::updateProfile(int id, const std::string& newName, float newHeight, float newWeight,
                         const std::string& newDob, const std::string& newUsername, const std::string& newPassword) {
    if (id == userID) {
        name = newName;
        height = newHeight;
        weight = newWeight;
        dob = newDob;
        username = newUsername;
        passwordHash = hashPassword(newPassword);
        std::cout << "User profile updated: ID " << userID
                  << ", New Name: " << name
                  << ", New Height: " << height << " cm"
                  << ", New Weight: " << weight << " kg"
                  << ", New DOB: " << dob
                  << ", New Username: " << username << "\n";
    } else {
        std::cout << "Error: User ID " << id << " does not match.\n";
    }
}

// Delete Profile
void User::deleteProfile() {
    userID = -1;
    name.clear();
    height = 0.0f;
    weight = 0.0f;
    dob.clear();
    username.clear();
    passwordHash.clear();
    profileData.clear();
    std::cout << "User profile has been deleted.\n";
}

// Delete Profile (collaborator)
void User::deleteProfile(int id) {
    if (id == userID) {
        deleteProfile();
    } else {
        std::cout << "Error: User ID " << id << " does not match.\n";
    }
}

// Retrieve historical data
std::vector<HealthData> User::retrieveHistoricalData(HistoricalDataManager* manager) {
    return manager->retrieveData(userID);
}

// Authentication
std::string User::getUsername() const { return username; }
bool User::verifyPassword(const std::string& password) const {
    return hashPassword(password) == passwordHash;
}

// Getters
int User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
float User::getHeight() const { return height; }
float User::getWeight() const { return weight; }
std::string User::getDob() const { return dob; }

// Calculate Age
int User::calculateAge() const {
    std::istringstream dobStream(dob);
    std::tm dobTm = {};
    dobStream >> std::get_time(&dobTm, "%Y-%m-%d");

    if (dobStream.fail()) {
        std::cerr << "Error parsing DOB\n";
        return -1;
    }

    std::time_t now = std::time(nullptr);
    std::tm nowTm = *std::localtime(&now);

    int age = nowTm.tm_year - dobTm.tm_year;
    if ((dobTm.tm_mon > nowTm.tm_mon) || (dobTm.tm_mon == nowTm.tm_mon && dobTm.tm_mday > nowTm.tm_mday)) {
        age--;
    }
    return age;
}
