#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

class BatteryManager {
private:
    float batteryLevel;

public:
    BatteryManager(float initialLevel);

    float checkBatteryLevel() const;
    void depleteBattery(float amount);
    void rechargeBattery(float amount);
};

#endif // BATTERYMANAGER_H