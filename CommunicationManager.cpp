#include "CommunicationManager.h"

void CommunicationManager::begin() {
    SerialBT.begin(BLUETOOTH_DEVICE_NAME);
}

bool CommunicationManager::isConnected() {
    return SerialBT.hasClient();
}

void CommunicationManager::sendLiveScanData(const SensorData& data) {
    if (!isConnected()) return;
    SerialBT.printf("{\"value\":%.1f, \"bat\":%.1f}\n", data.calibrated, data.battery);
}

void CommunicationManager::sendPointData(int x, int y, float value) {
    if (!isConnected()) return;
    SerialBT.printf("data,%d,%d,%.2f\n", x, y, value);
}

void CommunicationManager::sendGridSetupCommand(int width, int height) {
    if (!isConnected()) return;
    SerialBT.printf("grid,%d,%d\n", width, height);
}
