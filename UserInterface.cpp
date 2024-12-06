#include "UserInterface.h"
#include <iostream>

void UserInterface::displayMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}

void UserInterface::showMetrics() {
    std::cout << "Displaying Metrics..." << std::endl;
}

void UserInterface::displayWarning(const std::string& warning) {
    std::cout << "Warning: " << warning << std::endl;
}

void UserInterface::displayError(const std::string& error) {
    std::cout << "Error: " << error << std::endl;
}