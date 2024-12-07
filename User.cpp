#include "User.h"
#include "HistoricalDataManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Constructor
User::User(int id, const std::string& name, float height, float weight, const std::string& dob)
    : userID(id), name(name), height(height), weight(weight), dob(dob) {}

// Create Profile
void User::createProfile() {
    std::cout << "User profile created with ID: " << userID
              << ", Name: " << name
              << ", Height: " << height << " cm"
              << ", Weight: " << weight << " kg"
              << ", DOB: " << dob << "\n";
}

// Update Profile
void User::updateProfile(int id, const std::string& newName, float newHeight, float newWeight, const std::string& newDob) {
    if (id == userID) {
        name = newName;
        height = newHeight;
        weight = newWeight;
        dob = newDob;
        std::cout << "User profile updated: ID " << userID
                  << ", New Name: " << name
                  << ", New Height: " << height << " cm"
                  << ", New Weight: " << weight << " kg"
                  << ", New DOB: " << dob << "\n";
    } else {
        std::cout << "Error: User ID " << id << " does not match.\n";
    }
}

// Delete Profile
void User::deleteProfile(int id) {
    if (id == userID) {
        userID = -1;
        name.clear();
        height = 0.0f;
        weight = 0.0f;
        dob.clear();
        profileData.clear();
        std::cout << "User profile with ID " << id << " has been deleted.\n";
    } else {
        std::cout << "Error: User ID " << id << " does not match.\n";
    }
}

// Retrieve historical data
std::vector<HealthData> User::retrieveHistoricalData(HistoricalDataManager* manager) {
    return manager->retrieveData(userID);
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
