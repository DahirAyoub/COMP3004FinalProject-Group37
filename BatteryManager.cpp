#include "BatteryManager.h"

BatteryManager::BatteryManager(float initialLevel)
    : batteryLevel(initialLevel) {}

float BatteryManager::checkBatteryLevel() const {
    return batteryLevel;
}

void BatteryManager::depleteBattery(float amount) {
    batteryLevel -= amount;
    if (batteryLevel < 0) batteryLevel = 0;
}

void BatteryManager::rechargeBattery(float amount) {
    batteryLevel += amount;
    if (batteryLevel > 100) batteryLevel = 100;
}