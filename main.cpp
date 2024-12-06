#include "Device.h"
#include <iostream>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\nMenu Options:\n";
    std::cout << "1. Create User Profile\n";
    std::cout << "2. Update User Profile\n";
    std::cout << "3. Delete User Profile\n";
    std::cout << "4. Display All Profiles\n";
    std::cout << "5. Apply Device to Skin\n";
    std::cout << "6. Lift Device off Skin\n";
    std::cout << "7. Start Measurement\n";
    std::cout << "8. Exit\n";
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.clear(); // Clear error flag
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
}

void handleCreateUserProfile(Device& device) {
    int userID, age;
    std::string name;

    std::cout << "Enter User ID: ";
    while (!(std::cin >> userID)) {
        std::cout << "Invalid input. Please enter a valid User ID: ";
        clearInputBuffer();
    }

    std::cout << "Enter Name: ";
    clearInputBuffer(); // Clear input buffer for std::getline
    std::getline(std::cin, name);

    std::cout << "Enter Age: ";
    while (!(std::cin >> age)) {
        std::cout << "Invalid input. Please enter a valid age: ";
        clearInputBuffer();
    }

    device.createUserProfile(userID, name, age);
}

void handleUpdateUserProfile(Device& device) {
    int userID, newAge;
    std::string newName;

    std::cout << "Enter User ID to Update: ";
    while (!(std::cin >> userID)) {
        std::cout << "Invalid input. Please enter a valid User ID: ";
        clearInputBuffer();
    }

    std::cout << "Enter New Name: ";
    clearInputBuffer(); // Clear input buffer for std::getline
    std::getline(std::cin, newName);

    std::cout << "Enter New Age: ";
    while (!(std::cin >> newAge)) {
        std::cout << "Invalid input. Please enter a valid age: ";
        clearInputBuffer();
    }

    device.updateUserProfile(userID, newName, newAge);
}

void handleDeleteUserProfile(Device& device) {
    int userID;

    std::cout << "Enter User ID to Delete: ";
    while (!(std::cin >> userID)) {
        std::cout << "Invalid input. Please enter a valid User ID: ";
        clearInputBuffer();
    }

    device.deleteUserProfile(userID);
}

void handleDisplayAllProfiles(const Device& device) {
    std::cout << "\nAll User Profiles:\n";

    const std::vector<User>& profiles = device.getAllProfiles();
    if (profiles.empty()) {
        std::cout << "No profiles available.\n";
        return;
    }

    for (const auto& user : profiles) {
        std::cout << "User ID: " << user.getUserID()
                  << ", Name: " << user.getName()
                  << ", Age: " << user.getAge() << "\n";
    }
}

int main() {
    Device myDevice("RadoTechSim");
    int choice;

    do {
        displayMenu();
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number between 1 and 8: ";
            clearInputBuffer();
        }

        switch (choice) {
        case 1:
            handleCreateUserProfile(myDevice);
            break;
        case 2:
            handleUpdateUserProfile(myDevice);
            break;
        case 3:
            handleDeleteUserProfile(myDevice);
            break;
        case 4:
            handleDisplayAllProfiles(myDevice);
            break;
        case 5:
            myDevice.applyToSkin();
            break;
        case 6:
            myDevice.liftOffSkin();
            break;
        case 7:
            myDevice.startMeasurement();
            break;
        case 8:
            std::cout << "Exiting program. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}