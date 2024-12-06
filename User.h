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
    std::vector<HealthData> profileData;

public:
    User(int id, const std::string& name, int age);

    void createProfile();
    void updateProfile(int id, const std::string& newName, int newAge);
    void deleteProfile(int id);
    std::vector<HealthData> retrieveHistoricalData(HistoricalDataManager* manager);

    // Getters and setters
    int getUserID() const;
    void setName(const std::string& newName);
    void setAge(int newAge);
    std::string getName() const;
    int getAge() const;
};

#endif // USER_H
