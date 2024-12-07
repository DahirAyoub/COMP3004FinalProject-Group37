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
    // Constructor
    User(int id, const std::string& name, float height, float weight, const std::string& dob);

    void createProfile();
    void updateProfile(int id, const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void deleteProfile(int id);
    std::vector<HealthData> retrieveHistoricalData(HistoricalDataManager* manager);

    // Getters
    int getUserID() const;
    std::string getName() const;
    float getHeight() const;
    float getWeight() const;
    std::string getDob() const;

    // Age calculation
    int calculateAge() const;
};

#endif // USER_H