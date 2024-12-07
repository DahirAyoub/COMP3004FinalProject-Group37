#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "HealthData.h"

class HistoricalDataManager;

class User {
private:
    int userID;
    std::string name;
    float height; // Height in cm
    float weight; // Weight in kg
    std::string dob; // Date of Birth

    std::vector<HealthData> profileData;

public:
    // Default constructor
    User();

    // Parameterized constructor
    User(int id, const std::string& name, float height, float weight, const std::string& dob);

    // Profile management methods
    void updateProfile(const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void deleteProfile(); // Modified to no longer take userID
    std::vector<HealthData> retrieveHistoricalData(HistoricalDataManager* manager);

    // Getters
    int getUserID() const;
    std::string getName() const;
    float getHeight() const;
    float getWeight() const;
    std::string getDob() const;

    // Setters for field-specific updates
    void setName(const std::string& newName);
    void setHeight(float newHeight);
    void setWeight(float newWeight);
    void setDob(const std::string& newDob);

    // Age calculation
    int calculateAge() const;
};

#endif // USER_H
