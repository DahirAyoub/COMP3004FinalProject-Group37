#include "User.h"
#include "HistoricalDataManager.h"
#include <iostream>
#include <algorithm>

// Constructor
User::User(int id, const std::string& name, int age)
    : userID(id), name(name), age(age) {}

// Create Profile
void User::createProfile() {
    std::cout << "User profile created with ID: " << userID << ", Name: " << name << ", Age: " << age << "\n";
}

// Update Profile
void User::updateProfile(int id, const std::string& newName, int newAge) {
    if (id == userID) {
        name = newName;
        age = newAge;
        std::cout << "User profile updated: ID " << userID << ", New Name: " << name << ", New Age: " << age << "\n";
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

std::string User::getName() const {
    return name;
}

int User::getAge() const {
    return age;
}
