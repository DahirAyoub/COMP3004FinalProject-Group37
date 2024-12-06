#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include "BatteryManager.h"
#include "DataCollector.h"
#include "DataProcessor.h"
#include "MetricsVisualizer.h"
#include "HistoricalDataManager.h"

class Device {
private:
    std::string deviceID;
    float batteryLevel;
    std::string status;
    BatteryManager* batteryManager;
    DataCollector* dataCollector;
    DataProcessor* dataProcessor;
    MetricsVisualizer* visualizer;
    HistoricalDataManager* dataManager;

public:
    Device(const std::string& id);

    void startMeasurement();
    bool checkSkinContact();
    void depleteBattery();
    void displayMetrics();
    void storeProcessedData();
};

#endif // DEVICE_H