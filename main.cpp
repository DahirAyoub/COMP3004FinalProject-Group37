#include "Device.h"
#include "User.h"
#include <iostream>

int main() {
    // Create Device instance
    Device myDevice("RadoTechSim");

    // Test: Create up to five profiles
    std::cout << "Creating user profiles...\n";
    myDevice.createUserProfile(1, "Alice", 25);
    myDevice.createUserProfile(2, "Bob", 30);
    myDevice.createUserProfile(3, "Charlie", 28);
    myDevice.createUserProfile(4, "Diana", 22);
    myDevice.createUserProfile(5, "Eve", 29);

    // Test: Attempt to create a sixth profile (should fail)
    std::cout << "\nAttempting to create a sixth profile...\n";
    myDevice.createUserProfile(6, "Frank", 35);

    // Test: Update a profile
    std::cout << "\nUpdating profile for User ID 2...\n";
    myDevice.updateUserProfile(2, "Robert", 31);

    // Test: Delete a profile
    std::cout << "\nDeleting profile for User ID 3...\n";
    myDevice.deleteUserProfile(3);

    // Test: Try updating a deleted profile (should fail)
    std::cout << "\nAttempting to update deleted profile for User ID 3...\n";
    myDevice.updateUserProfile(3, "Charles", 29);

    // Test: Data collection
    std::cout << "\nTesting data collection...\n";
    myDevice.startDataCollection();

    return 0;
}