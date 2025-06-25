#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//================================================================
// 1. تعريف أطراف التوصيل (Hardware Pin Definitions)
//================================================================
//#define LED_BLUETOOTH       4
#define BUZZER_PIN          13
#define BUTTON_SCAN         26
#define FLC100_PIN          32
#define BATTERY_PIN         35
#define MPU_SDA             21
#define MPU_SCL             22
#define TFT_CS              5
#define TFT_DC              2
#define TFT_RST             4
#define XPT2046_CS          25 // يُستخدم داخليًا بواسطة TFT_eSPI

//================================================================
// 2. إعدادات النظام والتوقيت (System & Timing Settings)
//================================================================
#define SERIAL_BAUD_RATE      9600
#define MAIN_LOOP_DELAY       50   // تأخير بسيط للحفاظ على استجابة النظام
#define TOUCH_DEBOUNCE_TIME   300  // فترة تهدئة للمس (بالمللي ثانية)
#define LONG_PRESS_DURATION   1500 // مدة الضغطة الطويلة
#define BLUETOOTH_DEVICE_NAME "JUD X4"

//================================================================
// 3. حالات تشغيل الجهاز (State Machine)
//================================================================
enum DeviceState {
    STATE_HOME,
    STATE_LIVE_SCAN,
    STATE_GRID_SETUP,
    STATE_GRID_SCAN,
    STATE_GRID_COMPLETE,
    STATE_SCAN_SETTINGS,
    STATE_HISTORY,
    STATE_SAVE_SCAN,
    STATE_HISTORY_CONTEXT_MENU,
    STATE_VIEW_SAVED_SCAN,
};

//================================================================
// 4. هياكل البيانات (Data Structures)
//================================================================
struct ScanSettings {
    int sensitivity = 10;
    int gridWidth = 5;
    int gridHeight = 5;
};

struct SensorData {
    float raw = 0;
    float filtered = 0;
    float calibrated = 0;
    float battery = 0;
};

struct GridPoint {
    int x;
    int y;
};

#endif // CONFIG_H
