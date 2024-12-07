#include "User.h"
#include "HistoricalDataManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Default constructor
User::User() : userID(-1), name(""), height(0.0f), weight(0.0f), dob("") {}

// Parameterized constructor
User::User(int id, const std::string& name, float height, float weight, const std::string& dob)
    : userID(id), name(name), height(height), weight(weight), dob(dob) {}

// Update Profile (no longer requires userID verification since updates are managed by the current logged-in user)
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

// Delete Profile (modified to be handled at the `Device` level)
void User::deleteProfile() {
    userID = -1;
    name.clear();
    height = 0.0f;
    weight = 0.0f;
    dob.clear();
    profileData.clear();
    std::cout << "User profile has been deleted.\n";
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

// Setters for updating individual fields
void User::setName(const std::string& newName) { name = newName; }
void User::setHeight(float newHeight) { height = newHeight; }
void User::setWeight(float newWeight) { weight = newWeight; }
void User::setDob(const std::string& newDob) { dob = newDob; }

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
