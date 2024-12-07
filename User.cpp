#include "User.h"
#include "HistoricalDataManager.h"
#include <iostream>
#include <algorithm>

// Constructor
User::User(int id, const std::string& name, int age, float height, float weight, const std::string& dob)
    : userID(id), name(name), age(age), height(height), weight(weight), dob(dob) {}

// Create Profile
void User::createProfile() {
    std::cout << "User profile created with ID: " << userID
              << ", Name: " << name
              << ", Age: " << age
              << ", Height: " << height << " cm"
              << ", Weight: " << weight << " kg"
              << ", DOB: " << dob << "\n";
}

// Update Profile
void User::updateProfile(int id, const std::string& newName, int newAge, float newHeight, float newWeight, const std::string& newDob) {
    if (id == userID) {
        name = newName;
        age = newAge;
        height = newHeight;
        weight = newWeight;
        dob = newDob;
        std::cout << "User profile updated: ID " << userID
                  << ", New Name: " << name
                  << ", New Age: " << age
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
        userID = -1; // Invalid ID signifies deletion
        name = "";
        age = 0;
        height = 0.0f;
        weight = 0.0f;
        dob = "";
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

// Getters and setters
int User::getUserID() const {
    return userID;
}

void User::setName(const std::string& newName) {
    name = newName;
}

void User::setAge(int newAge) {
    age = newAge;
}

void User::setHeight(float newHeight) {
    height = newHeight;
}

void User::setWeight(float newWeight) {
    weight = newWeight;
}

void User::setDob(const std::string& newDob) {
    dob = newDob;
}

std::string User::getName() const {
    return name;
}

int User::getAge() const {
    return age;
}

float User::getHeight() const {
    return height;
}

float User::getWeight() const {
    return weight;
}

std::string User::getDob() const {
    return dob;
}
