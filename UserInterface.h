#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>

class UserInterface {
public:
    void displayMessage(const std::string& message);
    void showMetrics();
    void displayWarning(const std::string& warning);
    void displayError(const std::string& error);
};

#endif // USERINTERFACE_H