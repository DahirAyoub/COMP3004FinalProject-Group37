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
    int age;
    float height; // Height in cm
    float weight; // Weight in kg
    std::string dob; // Date of Birth

    std::vector<HealthData> profileData;

public:
    // Constructor
    User(int id, const std::string& name, int age, float height, float weight, const std::string& dob);

    void createProfile();
    void updateProfile(int id, const std::string& newName, int newAge, float newHeight, float newWeight, const std::string& newDob);
    void deleteProfile(int id);
    std::vector<HealthData> retrieveHistoricalData(HistoricalDataManager* manager);

    // Getters and setters
    int getUserID() const;
    void setName(const std::string& newName);
    void setAge(int newAge);
    void setHeight(float newHeight);
    void setWeight(float newWeight);
    void setDob(const std::string& newDob);

    std::string getName() const;
    int getAge() const;
    float getHeight() const;
    float getWeight() const;
    std::string getDob() const;
};

#endif // USER_H
