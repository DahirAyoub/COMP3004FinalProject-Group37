#include "Device.h"

Device::Device(const std::string& id)
    : deviceID(id), batteryLevel(100.0f), status("Idle"),
      batteryManager(new BatteryManager(100.0f)),
      dataCollector(new DataCollector()),
      dataProcessor(new DataProcessor()),
      visualizer(new MetricsVisualizer("Chart")),
      dataManager(new HistoricalDataManager()) {}

void Device::startMeasurement() {
    if (batteryManager->checkBatteryLevel() < 20.0f) {
        status = "Low Battery";
        return;
    }

    if (!checkSkinContact()) {
        status = "Error";
        return;
    }

    auto rawData = dataCollector->collectData();
    auto metrics = dataProcessor->processRawData(rawData);
    visualizer->displayMetrics(metrics);
    storeProcessedData();
}

bool Device::checkSkinContact() {
    // Placeholder for actual skin contact detection logic
    return true;
}

void Device::depleteBattery() {
    batteryManager->depleteBattery(5.0f); // Depletes battery by 5%
}

void Device::displayMetrics() {
    // Displays previously processed metrics
}

void Device::storeProcessedData() {
    // Stores processed metrics into historical data
}