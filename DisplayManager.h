#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>
#include "config.h"

class HistoryManager;
class CommunicationManager;

class DisplayManager {
public:
    DisplayManager();
    bool begin();
    void update(DeviceState state, const SensorData& data, const ScanSettings& settings,
                bool btConnected, const GridPoint& gridPos, const float gridValues[10][10],
                HistoryManager& historyMgr, const char* selectedFile, bool isGridCalibrated, bool mpuAvailable, bool gridScanStarted);
    DeviceState handleTouch(DeviceState currentState, ScanSettings& settings, HistoryManager& historyMgr, char* selectedFile, CommunicationManager& comms);
    void showFatalError(const char* message);
    void showWelcomeScreen();
    void showCalibratingScreen();

private:
    TFT_eSPI tft;
    DeviceState previousState;
    uint16_t calData[5] = { 261, 3545, 355, 3417, 1 };
    bool touchIsActive;
    unsigned long touchStartTime;
    unsigned long lastTouchTime;
    bool longPressActionDone;
    int selectedHistoryItem;
    uint16_t last_t_x, last_t_y;
    unsigned long lastLiveScanUpdate;
    float previousValue;

    void drawHomeScreen();
    void drawLiveScanScreen(const SensorData& data, bool mpuAvailable);
    void drawGridSetupScreen(const ScanSettings& settings);
    void drawGridScanScreen(const GridPoint& gridPos, const ScanSettings& settings, const float gridValues[10][10], bool isGridCalibrated, bool mpuAvailable);
    void drawGridCompleteScreen();
    void drawSettingsScreen(const ScanSettings& settings);
    void drawHistoryScreen(HistoryManager& historyMgr);
    void drawHistoryContextMenu(HistoryManager& historyMgr, const char* filename);
    void drawViewScanScreen(HistoryManager& historyMgr, const char* filename);

    void drawStatusBar(const SensorData& data, bool btConnected);
    void drawBackButton();
    void drawHomeCard(int x, int y, int w, int h, const char* title, const char* subtitle, uint16_t color);

    void drawColorGradientBackground();
    void updateNeedleAndValue(float value);
    void drawRadialGauge(float value);
    uint16_t getGaugeColor(float value);
    uint16_t calculateHeatmapColor(float value);

    DeviceState handleHomeTouch(uint16_t x, uint16_t y);
    DeviceState handleGridSetupTouch(uint16_t x, uint16_t y, ScanSettings& settings);
    DeviceState handleGridCompleteTouch(uint16_t x, uint16_t y);
    DeviceState handleSettingsTouch(uint16_t x, uint16_t y, ScanSettings& settings);
    DeviceState handleHistoryTouch(uint16_t x, uint16_t y, HistoryManager& historyMgr, char* selectedFile);
    DeviceState handleContextMenuTouch(uint16_t x, uint16_t y, HistoryManager& historyMgr, const char* filename, CommunicationManager& comms);
    DeviceState handleViewScanTouch(uint16_t x, uint16_t y);
};

#endif // DISPLAY_MANAGER_H
