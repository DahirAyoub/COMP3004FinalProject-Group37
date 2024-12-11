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
    float height;       // in cm
    float weight;       // in kg
    std::string dob;    // YYYY-MM-DD

    std::string username;
    std::string passwordHash;

    std::vector<HealthData> profileData;

    // Placeholder password hashing
    std::string hashPassword(const std::string& password) const;

public:
    // Default constructor
    User();

    // Constructor without username/password
    User(int id, const std::string& name, float height, float weight, const std::string& dob);

    // Constructor with username/password
    User(int id, const std::string& name, float height, float weight, const std::string& dob,
         const std::string& username, const std::string& password);

    // Profile management
    void createProfile();
    void updateProfile(const std::string& newName, float newHeight, float newWeight, const std::string& newDob);
    void updateProfile(int id, const std::string& newName, float newHeight, float newWeight,
                       const std::string& newDob, const std::string& newUsername, const std::string& newPassword);
    void deleteProfile();
    void deleteProfile(int id);
    std::vector<HealthData> retrieveHistoricalData(HistoricalDataManager* manager);

    // Authentication
    std::string getUsername() const;
    bool verifyPassword(const std::string& password) const;

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