#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H
#define FILTER_SIZE 8

#include <MPU9250_WE.h>
#include <Wire.h>
#include "config.h"
#include "FilterSystem.h"

class SensorManager {
public:
    SensorManager();
    void begin(); // لن توقف البرنامج بعد الآن
    SensorData read(const ScanSettings& settings);
    void calibrate();
    bool isMpuAvailable(); // دالة جديدة لمعرفة حالة الحساس

private:
    MPU9250_WE mpu;
    FilterSystem filterSys;
    float flcCalibrationValue;
    bool mpuAvailable; // متغير لتخزين حالة الحساس
    float filterBuffer[FILTER_SIZE];
    int filterIndex;
    float filterSum;
    int filterCount;
};

#endif // SENSOR_MANAGER_H
