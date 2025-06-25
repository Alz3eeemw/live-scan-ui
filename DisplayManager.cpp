#include "DisplayManager.h"
#include "HistoryManager.h"
#include "CommunicationManager.h"
#include <math.h>

#define COLOR_BACKGROUND 0x0842
#define COLOR_CARD_BLUE 0x041F
#define COLOR_TOP_BAR 0x041F
#define LIVE_SCAN_UPDATE_INTERVAL 100

void hsvToRgb(float h, float s, float v, uint8_t* r, uint8_t* g, uint8_t* b) {
    float i = floor(h / 60.0f);
    float f = h / 60.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    int mod = (int)i % 6;
    switch (mod) {
        case 0: *r = v * 255; *g = t * 255; *b = p * 255; break;
        case 1: *r = q * 255; *g = v * 255; *b = p * 255; break;
        case 2: *r = p * 255; *g = v * 255; *b = t * 255; break;
        case 3: *r = p * 255; *g = q * 255; *b = v * 255; break;
        case 4: *r = t * 255; *g = p * 255; *b = v * 255; break;
        case 5: *r = v * 255; *g = p * 255; *b = q * 255; break;
    }
}

DisplayManager::DisplayManager() :
    tft(TFT_eSPI()),
    previousState((DeviceState)-1),
    touchIsActive(false),
    touchStartTime(0),
    lastTouchTime(0),
    longPressActionDone(false),
    selectedHistoryItem(-1),
    last_t_x(0), last_t_y(0),
    lastLiveScanUpdate(0),
    previousValue(-999.0f)
{}

bool DisplayManager::begin() {
    tft.init();
    tft.setRotation(1);
    tft.setTouch(calData);
    return true;
}

void DisplayManager::update(DeviceState state, const SensorData& data, const ScanSettings& settings,
                            bool btConnected, const GridPoint& gridPos, const float gridValues[10][10],
                            HistoryManager& historyMgr, const char* selectedFile, bool isGridCalibrated, bool mpuAvailable, bool gridScanStarted) {
    tft.startWrite();
    if (state != previousState) {
        tft.fillScreen(COLOR_BACKGROUND);
        previousValue = -999.0f;
        switch (state) {
            case STATE_HOME: drawHomeScreen(); break;
            case STATE_GRID_SETUP: drawGridSetupScreen(settings); break;
            case STATE_GRID_COMPLETE: drawGridCompleteScreen(); break;
            case STATE_SCAN_SETTINGS: drawSettingsScreen(settings); break;
            case STATE_HISTORY: drawHistoryScreen(historyMgr); break;
            case STATE_HISTORY_CONTEXT_MENU: drawHistoryContextMenu(historyMgr, selectedFile); break;
            case STATE_VIEW_SAVED_SCAN: drawViewScanScreen(historyMgr, selectedFile); break;
            default: break;
        }
    }

    if (state == STATE_LIVE_SCAN) drawLiveScanScreen(data, mpuAvailable);
    if (state == STATE_GRID_SCAN) drawGridScanScreen(gridPos, settings, gridValues, isGridCalibrated, mpuAvailable);

    drawStatusBar(data, btConnected);
    if (state != STATE_HOME) drawBackButton();
    previousState = state;
    tft.endWrite();
}

void DisplayManager::drawLiveScanScreen(const SensorData& data, bool mpuAvailable) {
    static bool backgroundDrawn = false;
    if (previousState != STATE_LIVE_SCAN) backgroundDrawn = false;
    if (!backgroundDrawn) {
        drawColorGradientBackground();
        backgroundDrawn = true;
        tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Value:", 20, 185, 2);
        tft.drawString("Depth:", 20, 205, 2);
        tft.setTextDatum(TR_DATUM);
        tft.drawString("Conf:", 220, 185, 2);
        tft.drawString("Type:", 220, 205, 2);
    }
    if (millis() - lastLiveScanUpdate > LIVE_SCAN_UPDATE_INTERVAL) {
        lastLiveScanUpdate = millis();
        updateNeedleAndValue(data.calibrated);
        tft.setTextColor(TFT_BLACK);
        tft.setTextDatum(TL_DATUM);
        tft.drawString("Value:", 250, 70, 2);
        tft.drawFloat(data.calibrated, 1, 300, 70, 2);
        tft.drawString("Depth:", 250, 100, 2);
        float depth = (abs(data.calibrated) > 10) ? (200.0 / abs(data.calibrated)) : 0;
        tft.drawFloat(depth, 1, 300, 100, 2);
        tft.drawString("Conf:", 250, 130, 2);
        float conf = constrain(map(abs(data.calibrated), 0, 200, 0, 100), 0, 100);
        tft.drawFloat(conf, 1, 300, 130, 2);
        tft.drawString("Type:", 250, 160, 2);
        if (data.calibrated > 20) tft.drawString("Metal", 300, 160, 2);
        else if (data.calibrated < -20) tft.drawString("Void", 300, 160, 2);
        else tft.drawString("Ground", 300, 160, 2);
    }
    if (!mpuAvailable) {
        tft.setTextColor(TFT_YELLOW, COLOR_BACKGROUND);
        tft.setTextDatum(BC_DATUM);
        tft.drawString("No MPU: Tilt OFF", 160, 235, 2);
    }
}

void DisplayManager::drawColorGradientBackground() {
    int x = 160, y = 100, r_outer = 85, r_inner = 65, start_angle = 225, end_angle = -45;
    for (int angle = start_angle; angle >= end_angle; angle -= 2) {
        float mappedValue = map(angle, start_angle, end_angle, -200, 200);
        uint16_t color = getGaugeColor(mappedValue);
        tft.drawWedgeLine(x, y, r_inner, r_outer, angle, angle - 2, color);
    }
}

void DisplayManager::updateNeedleAndValue(float value) {
    static float prevAngle = -9999;
    int x = 160, y = 100, r_outer = 85, r_inner = 65, start_angle = 225, end_angle = -45;
    if (prevAngle != -9999) {
        float prevRad = prevAngle * DEG_TO_RAD;
        int px1 = x + (r_outer - 5) * cos(prevRad);
        int py1 = y + (r_outer - 5) * sin(prevRad);
        float mappedPrev = map(prevAngle, start_angle, end_angle, -200, 200);
        float prevHue = (mappedPrev < 0) ? map(mappedPrev, -200, 0, 240, 120) : map(mappedPrev, 0, 200, 120, 0);
        uint8_t pr, pg, pb;
        hsvToRgb(prevHue, 1.0f, 1.0f, &pr, &pg, &pb);
        uint16_t prevColor = tft.color565(pr, pg, pb);
        tft.drawLine(x, y, px1, py1, prevColor);
    }
    float constrainedValue = constrain(value, -200, 200);
    float needleAngle = map(constrainedValue, -200, 200, start_angle, end_angle);
    float rad = needleAngle * DEG_TO_RAD;
    int x1 = x + (r_outer - 5) * cos(rad);
    int y1 = y + (r_outer - 5) * sin(rad);
    tft.drawLine(x, y, x1, y1, TFT_RED);
    tft.setTextColor(TFT_BLACK, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawFloat(value, 1, x, y, 4);
    prevAngle = needleAngle;
}

void DisplayManager::drawRadialGauge(float value) {
    int x = 160, y = 100, r_outer = 85, r_inner = 65, start_angle = 225, end_angle = -45;
    if (previousState != STATE_LIVE_SCAN) {
        for (int i = start_angle; i >= end_angle; i -= 2) {
            float segment_value = map(i, start_angle, end_angle, -100, 100);
            uint16_t color = getGaugeColor(segment_value);
            tft.drawWedgeLine(x, y, r_inner, r_outer, i, i - 2, color);
        }
    }
    tft.fillCircle(x, y, r_inner - 2, COLOR_BACKGROUND);
    float needle_angle_deg = map(constrain(value, -100, 100), -100, 100, start_angle, end_angle);
    float needle_angle_rad = needle_angle_deg * DEG_TO_RAD;
    int x1 = x + (r_outer - 5) * cos(needle_angle_rad);
    int y1 = y + (r_outer - 5) * sin(needle_angle_rad);
    tft.drawLine(x, y, x1, y1, TFT_RED);
    tft.drawLine(x + 1, y, x1 + 1, y1, TFT_RED);
    tft.fillCircle(x, y, 35, TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.drawFloat(value, 0, x, y, 4);
}

uint16_t DisplayManager::getGaugeColor(float value) {
    float hue = (value < 0) ? map(value, -200, 0, 240, 120) : map(value, 0, 200, 120, 0);
    uint8_t r, g, b;
    hsvToRgb(hue, 1.0f, 1.0f, &r, &g, &b);
    return tft.color565(r, g, b);
}

DeviceState DisplayManager::handleTouch(DeviceState currentState, ScanSettings& settings, HistoryManager& historyMgr, char* selectedFile, CommunicationManager& comms) {
    uint16_t t_x, t_y;
    bool pressed = tft.getTouch(&t_x, &t_y);

    if (pressed) {
        if (!touchIsActive) {
            touchIsActive = true;
            longPressActionDone = false;
            touchStartTime = millis();
            last_t_x = t_x; 
            last_t_y = t_y;
        }

        if (currentState == STATE_HISTORY && (millis() - touchStartTime > LONG_PRESS_DURATION) && !longPressActionDone) {
            longPressActionDone = true;
            return handleHistoryTouch(last_t_x, last_t_y, historyMgr, selectedFile);
        }
    } else {
        if (touchIsActive) {
            if ((millis() - lastTouchTime > TOUCH_DEBOUNCE_TIME)) {
                if (!longPressActionDone) {
                    // Back button logic for Live Scan
                    if (currentState == STATE_LIVE_SCAN && last_t_x > 10 && last_t_x < 80 && last_t_y > 205) {
                        return STATE_HOME;
                    }
                    
                    // General back button for other screens
                    if (currentState != STATE_HOME && last_t_x > 10 && last_t_x < 80 && last_t_y > 205) {
                        if (currentState != STATE_GRID_SCAN) return STATE_HOME;
                    }
                    
                    switch (currentState) {
                        case STATE_HOME:                 return handleHomeTouch(last_t_x, last_t_y);
                        case STATE_GRID_SETUP:           return handleGridSetupTouch(last_t_x, last_t_y, settings);
                        case STATE_GRID_COMPLETE:        return handleGridCompleteTouch(last_t_x, last_t_y);
                        case STATE_SCAN_SETTINGS:        return handleSettingsTouch(last_t_x, last_t_y, settings);
                        case STATE_HISTORY:              return handleHistoryTouch(last_t_x, last_t_y, historyMgr, selectedFile);
                        case STATE_HISTORY_CONTEXT_MENU: return handleContextMenuTouch(last_t_x, last_t_y, historyMgr, selectedFile, comms);
                        case STATE_VIEW_SAVED_SCAN:      return handleViewScanTouch(last_t_x, last_t_y);
                        default: break;
                    }
                }
                lastTouchTime = millis();
            }
            touchIsActive = false;
        }
    }
    return currentState;
}

void DisplayManager::showCalibratingScreen() {
    tft.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Calibrating...", 160, 120, 4);
}

void DisplayManager::drawStatusBar(const SensorData& data, bool btConnected) {
    tft.fillRect(0, 0, 320, 24, COLOR_TOP_BAR);
    uint16_t btColor = btConnected ? TFT_BLUE : TFT_RED;
    tft.fillCircle(300, 12, 5, btColor);

    int batPercentage = constrain(map(data.battery, 3.2, 4.2, 0, 100), 0, 100);
    uint16_t batColor = (batPercentage > 50) ? TFT_GREEN : (batPercentage > 20) ? TFT_YELLOW : TFT_RED;
    tft.drawRect(10, 6, 30, 12, TFT_WHITE);
    tft.fillRect(11, 7, 28 * batPercentage / 100, 10, batColor);

    tft.setTextColor(TFT_WHITE, COLOR_TOP_BAR);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(String(batPercentage) + "%", 280, 12, 2);
}

void DisplayManager::drawBackButton() {
    tft.fillRoundRect(10, 205, 70, 30, 5, COLOR_CARD_BLUE);
    tft.setTextColor(TFT_WHITE, COLOR_CARD_BLUE);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Back", 45, 220, 2);
}

void DisplayManager::drawHomeScreen() {
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("JUD X4 PRO", 160, 45, 4);

    drawHomeCard(20, 80, 130, 100, "Live Scan", "Real-time sensor", COLOR_CARD_BLUE);
    drawHomeCard(170, 80, 130, 100, "Grid Scan", "2D/3D mapping", COLOR_CARD_BLUE);
    drawHomeCard(20, 190, 130, 40, "History", "View saved scans", COLOR_CARD_BLUE);
    drawHomeCard(170, 190, 130, 40, "Settings", "Adjust device", COLOR_CARD_BLUE);
}

void DisplayManager::drawHomeCard(int x, int y, int w, int h, const char* title, const char* subtitle, uint16_t color) {
    tft.fillRoundRect(x, y, w, h, 8, color);
    tft.setTextColor(TFT_WHITE, color);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(title, x + w / 2, y + h / 2 - 10, 2);
    tft.setTextColor(TFT_LIGHTGREY, color);
    tft.drawString(subtitle, x + w / 2, y + h / 2 + 10, 2);
}

void DisplayManager::drawGridSetupScreen(const ScanSettings& settings) {
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Grid Setup", 160, 45, 4);

    tft.drawString("Width:", 80, 100, 2);
    tft.drawString(String(settings.gridWidth), 160, 100, 4);

    tft.drawString("Height:", 80, 150, 2);
    tft.drawString(String(settings.gridHeight), 160, 150, 4);

    tft.fillRoundRect(220, 85, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("+", 240, 105, 4);
    tft.fillRoundRect(270, 85, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("-", 290, 105, 4);
    tft.fillRoundRect(220, 135, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("+", 240, 155, 4);
    tft.fillRoundRect(270, 135, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("-", 290, 155, 4);

    tft.fillRoundRect(100, 205, 120, 30, 5, TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.drawString("Start Scan", 160, 220, 2);
}

void DisplayManager::drawGridScanScreen(const GridPoint& gridPos, const ScanSettings& settings, const float gridValues[10][10], bool isGridCalibrated, bool mpuAvailable) {
    if (previousState != STATE_GRID_SCAN) {
        tft.drawRect(20, 30, 280, 200, TFT_WHITE);
    }

    int cellW = 280 / settings.gridWidth;
    int cellH = 200 / settings.gridHeight;

    for (int y = 0; y < settings.gridHeight; y++) {
        for (int x = 0; x < settings.gridWidth; x++) {
            tft.fillRect(20 + x * cellW, 30 + y * cellH, cellW, cellH, calculateHeatmapColor(gridValues[y][x]));
            if (x == gridPos.x && y == gridPos.y) {
                tft.drawRect(20 + x * cellW, 30 + y * cellH, cellW, cellH, TFT_YELLOW);
            }
        }
    }

    if (!isGridCalibrated) {
        tft.fillRect(60, 110, 200, 40, TFT_DARKGREY);
        tft.setTextColor(TFT_CYAN, TFT_DARKGREY);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Press button to calibrate", 160, 130, 2);
    }

    if (!mpuAvailable) {
        tft.setTextColor(TFT_YELLOW, COLOR_BACKGROUND);
        tft.setTextDatum(BC_DATUM);
        tft.drawString("No MPU: Tilt OFF", 160, 235, 2);
    }
}

uint16_t DisplayManager::calculateHeatmapColor(float value) {
    if (value == 0) return 0x2104;

    uint8_t r, g, b;
    if (value > 0) {
        float perc = value / 100.0;
        if (perc < 0.5) { r = map(perc, 0, 0.5, 0, 255); g = 255; b = 0; }
        else { r = 255; g = map(perc, 0.5, 1.0, 255, 0); b = 0; }
    } else {
        float perc = abs(value) / 100.0;
        if (perc < 0.5) { r = 0; g = 255; b = map(perc, 0, 0.5, 0, 255); }
        else { r = 0; g = map(perc, 0.5, 1.0, 255, 0); b = 255; }
    }
    return tft.color565(r, g, b);
}

void DisplayManager::drawGridCompleteScreen() {
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Scan Complete!", 160, 60, 4);

    tft.fillRoundRect(40, 120, 100, 50, 5, TFT_GREEN);
    tft.drawString("Save", 90, 145, 2);

    tft.fillRoundRect(180, 120, 100, 50, 5, TFT_RED);
    tft.drawString("Discard", 230, 145, 2);
}

void DisplayManager::drawHistoryScreen(HistoryManager& historyMgr) {
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Saved Scans", 160, 30, 4);

    auto files = historyMgr.listScans();
    int yPos = 70;
    for (const auto& file : files) {
        tft.fillRoundRect(20, yPos, 280, 30, 5, COLOR_CARD_BLUE);
        tft.setTextColor(TFT_WHITE, COLOR_CARD_BLUE);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(file.c_str(), 160, yPos + 15, 2);
        yPos += 35;
        if (yPos > 180) break;
    }
}

void DisplayManager::drawHistoryContextMenu(HistoryManager& historyMgr, const char* filename) {
    drawHistoryScreen(historyMgr);
    tft.fillRect(60, 50, 200, 140, TFT_DARKGREY);
    tft.drawRect(60, 50, 200, 140, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(filename, 160, 65, 2);

    tft.drawRect(70, 85, 180, 30, TFT_CYAN);
    tft.drawString("View Scan", 160, 100, 2);

    tft.drawRect(70, 120, 180, 30, TFT_GREEN);
    tft.drawString("Send to PC", 160, 135, 2);

    tft.drawRect(70, 155, 180, 30, TFT_RED);
    tft.drawString("Delete", 160, 170, 2);
}

void DisplayManager::drawViewScanScreen(HistoryManager& historyMgr, const char* filename) {
    ScanSettings settings;
    float gridValues[10][10];
    if (historyMgr.loadScan(filename, gridValues, settings)) {
        drawGridScanScreen(GridPoint{-1, -1}, settings, gridValues, true, true);
    }
}

void DisplayManager::drawSettingsScreen(const ScanSettings& settings) {
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Settings", 160, 45, 4);

    tft.drawString("Sensitivity:", 80, 120, 2);
    tft.fillRect(140, 100, 60, 40, COLOR_BACKGROUND);
    tft.drawString(String(settings.sensitivity), 160, 120, 4);

    tft.fillRoundRect(220, 105, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("+", 240, 125, 4);
    tft.fillRoundRect(270, 105, 40, 40, 5, COLOR_CARD_BLUE); tft.drawString("-", 290, 125, 4);
}

void DisplayManager::showFatalError(const char* message) {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("FATAL ERROR", 160, 100, 4);
    tft.drawString(message, 160, 140, 2);
    while(1);
}

void DisplayManager::showWelcomeScreen() {
    tft.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("JUD X4 PRO", 160, 110, 4);
    tft.drawString("Starting...", 160, 150, 2);
}

DeviceState DisplayManager::handleHomeTouch(uint16_t x, uint16_t y) {
    if (y > 80 && y < 180) {
        if (x > 20 && x < 150) return STATE_LIVE_SCAN;
        if (x > 170 && x < 300) return STATE_GRID_SETUP;
    }
    if (y > 190 && y < 230) {
        if (x > 20 && x < 150) return STATE_HISTORY;
        if (x > 170 && x < 300) return STATE_SCAN_SETTINGS;
    }
    return STATE_HOME;
}

DeviceState DisplayManager::handleGridSetupTouch(uint16_t x, uint16_t y, ScanSettings& settings) {
    bool changed = false;
    if (y > 85 && y < 125) {
        if (x > 220 && x < 260) { settings.gridWidth = min(10, settings.gridWidth + 1); changed = true; }
        if (x > 270 && x < 310) { settings.gridWidth = max(2, settings.gridWidth - 1); changed = true; }
    }
    if (y > 135 && y < 175) {
        if (x > 220 && x < 260) { settings.gridHeight = min(10, settings.gridHeight + 1); changed = true; }
        if (x > 270 && x < 310) { settings.gridHeight = max(2, settings.gridHeight - 1); changed = true; }
    }
    if (changed) {
        drawGridSetupScreen(settings);
    }
    if (y > 205 && y < 235 && x > 100 && x < 220) {
        return STATE_GRID_SCAN;
    }
    return STATE_GRID_SETUP;
}

DeviceState DisplayManager::handleGridCompleteTouch(uint16_t x, uint16_t y) {
    if (x > 40 && x < 140 && y > 120 && y < 170) return STATE_SAVE_SCAN;
    if (x > 180 && x < 280 && y > 120 && y < 170) return STATE_HOME;
    return STATE_GRID_COMPLETE;
}

DeviceState DisplayManager::handleHistoryTouch(uint16_t x, uint16_t y, HistoryManager& historyMgr, char* selectedFile) {
    auto files = historyMgr.listScans();
    int yPos = 70;
    for (size_t i = 0; i < files.size(); ++i) {
        if (y > yPos && y < yPos + 30) {
            strcpy(selectedFile, files[i].c_str());
            selectedHistoryItem = i;
            return longPressActionDone ? STATE_HISTORY_CONTEXT_MENU : STATE_VIEW_SAVED_SCAN;
        }
        yPos += 35;
        if (yPos > 180) break;
    }
    return STATE_HISTORY;
}

DeviceState DisplayManager::handleContextMenuTouch(uint16_t x, uint16_t y, HistoryManager& historyMgr, const char* filename, CommunicationManager& comms) {
    if (x > 70 && x < 250) {
        if (y > 85 && y < 115) return STATE_VIEW_SAVED_SCAN;
        if (y > 120 && y < 150) { historyMgr.sendScan(filename, comms); return STATE_HISTORY; }
        if (y > 155 && y < 185) { historyMgr.deleteScan(filename); return STATE_HISTORY; }
    }
    return STATE_HISTORY;
}

DeviceState DisplayManager::handleViewScanTouch(uint16_t x, uint16_t y) {
    return STATE_HISTORY;
}

DeviceState DisplayManager::handleSettingsTouch(uint16_t x, uint16_t y, ScanSettings& settings) {
    bool changed = false;
    if (y > 105 && y < 145) {
        if (x > 220 && x < 260) { settings.sensitivity = min(20, settings.sensitivity + 1); changed = true; }
        if (x > 270 && x < 310) { settings.sensitivity = max(1, settings.sensitivity - 1); changed = true; }
    }
    if (changed) {
        drawSettingsScreen(settings);
    }
    return STATE_SCAN_SETTINGS;
}
