#include "config.h"
#include "SensorManager.h"
#include "DisplayManager.h"
#include "CommunicationManager.h"
#include "HistoryManager.h"

SensorManager         sensorMgr;
DisplayManager        displayMgr;
CommunicationManager  commMgr;
HistoryManager        historyMgr;

DeviceState           currentState = STATE_HOME;
ScanSettings          scanSettings;
SensorData            sensorData;
float                 gridValues[10][10] = {0};
GridPoint             currentGridPoint = {0, 0};
bool                  isGridCalibrated = false;
bool                  gridScanStarted = false;
char                  selectedFile[30];

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
    sensorMgr.begin();
    if (!displayMgr.begin()) handleFatalError("Display Init Failed");
    commMgr.begin();
    historyMgr.begin();
    pinMode(BUTTON_SCAN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    //pinMode(LED_BLUETOOTH, OUTPUT);
    displayMgr.showWelcomeScreen();
    playStartupSound();
    delay(2000);
}

void loop() {
    DeviceState previousState = currentState;
    DeviceState newStateFromTouch = displayMgr.handleTouch(currentState, scanSettings, historyMgr, selectedFile, commMgr);

    if (newStateFromTouch != currentState) {
        currentState = newStateFromTouch;
        
        if (currentState == STATE_LIVE_SCAN || currentState == STATE_GRID_SCAN) {
            displayMgr.showCalibratingScreen();
            sensorMgr.calibrate();
            delay(500);
        }

        if (currentState == STATE_GRID_SCAN) {
            resetGridScan();
            commMgr.sendGridSetupCommand(scanSettings.gridWidth, scanSettings.gridHeight);
        }
    }

    sensorData = sensorMgr.read(scanSettings);
    bool mpuStatus = sensorMgr.isMpuAvailable();

    switch (currentState) {
        case STATE_LIVE_SCAN:
            handleLiveScanState();
            break;
        case STATE_GRID_SCAN:
            handleGridScanState();
            break;
        case STATE_GRID_COMPLETE:
            handleGridCompleteState();
            break;
        case STATE_SAVE_SCAN:
             historyMgr.saveScan(gridValues, scanSettings);
             currentState = STATE_HOME;
             break;
        default:
            break;
    }

    bool btConnected = commMgr.isConnected();
   // digitalWrite(LED_BLUETOOTH, btConnected ? HIGH : LOW);
    displayMgr.update(currentState, sensorData, scanSettings, btConnected, currentGridPoint, gridValues, historyMgr, selectedFile, isGridCalibrated, mpuStatus, gridScanStarted);

    delay(MAIN_LOOP_DELAY);
}

void handleLiveScanState() {
    commMgr.sendLiveScanData(sensorData);
}

void handleGridScanState() {
    if (digitalRead(BUTTON_SCAN) == LOW) {
        delay(50);
        if (digitalRead(BUTTON_SCAN) == LOW) {
            if (!isGridCalibrated) {
                displayMgr.showCalibratingScreen();
                sensorMgr.calibrate();
                isGridCalibrated = true;
            } else {
                scanCurrentGridPoint();
            }
            while (digitalRead(BUTTON_SCAN) == LOW);
        }
    }
}

void handleGridCompleteState() {
    if (!gridScanStarted) {
        currentState = STATE_HOME;
    }
}

void handleHomeState() { }
void handleGridSetupState() { }
void handleSettingsState() { }
void handleHistoryState() { }
void handleViewScanState() { }

void scanCurrentGridPoint() {
    gridScanStarted = true;
    tone(BUZZER_PIN, 3000, 100);
    SensorData pointData = sensorMgr.read(scanSettings);
    gridValues[currentGridPoint.y][currentGridPoint.x] = pointData.calibrated;
    commMgr.sendPointData(currentGridPoint.x, currentGridPoint.y, pointData.calibrated);
    
    currentGridPoint.x++;
    if (currentGridPoint.x >= scanSettings.gridWidth) {
        currentGridPoint.x = 0;
        currentGridPoint.y++;
    }
    if (currentGridPoint.y >= scanSettings.gridHeight) {
        currentState = STATE_GRID_COMPLETE;
    }
}

void resetGridScan() {
    isGridCalibrated = false;
    gridScanStarted = false;
    currentGridPoint = {0, 0};
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            gridValues[y][x] = 0.0f;
        }
    }
}

void handleFatalError(const char* message) {
    displayMgr.showFatalError(message);
    while (true) {
        tone(BUZZER_PIN, 3000, 500);
        delay(1000);
    }
}

void playStartupSound() {
    tone(BUZZER_PIN, 2000, 150);
    delay(200);
    tone(BUZZER_PIN, 2500, 150);
}
