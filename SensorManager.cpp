#include "SensorManager.h"

SensorManager::SensorManager() : mpu(&Wire, 0x68), flcCalibrationValue(1.65f), mpuAvailable(false), filterIndex(0), filterSum(0.0f), filterCount(0) {
    for (int i = 0; i < FILTER_SIZE; ++i) filterBuffer[i] = 0.0f;
}


void SensorManager::begin() {
    Wire.begin(MPU_SDA, MPU_SCL);
    if (mpu.init()) {
        mpu.enableAccDLPF(true);
        mpu.enableGyrDLPF();
        mpuAvailable = true;
        Serial.println("MPU6500 Initialized. Tilt Correction is ON.");
    } else {
        mpuAvailable = false;
        Serial.println("WARNING: MPU6500 not found. Tilt Correction is OFF.");
    }
}

SensorData SensorManager::read(const ScanSettings& settings) {
    SensorData data;
    float cos_tilt = 1.0f;
    if (mpuAvailable) {
        xyzFloat gValue = mpu.getGValues();
        float ax = gValue.x, ay = gValue.y, az = gValue.z;
        float g_total = sqrt(ax * ax + ay * ay + az * az);
        if (g_total > 0.1f) cos_tilt = az / g_total;
        if (abs(cos_tilt) < 0.15f) cos_tilt = (cos_tilt > 0) ? 0.15f : -0.15f;
    }
    int rawADC = analogRead(FLC100_PIN);
    data.raw = rawADC * (3.3f / 4095.0f);
    float corrected_raw = data.raw / cos_tilt;
    data.filtered = filterSys.processRealtime(corrected_raw);
    float sensitivityMultiplier = settings.sensitivity / 10.0f;
    float diff = (data.filtered - flcCalibrationValue) * sensitivityMultiplier;
    data.calibrated = constrain(diff * 1000, -100, 100);
    data.battery = analogRead(BATTERY_PIN) * (3.3f / 4095.0f) * 2.0f;
    return data;
}

void SensorManager::calibrate() {
    const int calibrationSamples = 100;
    float sum = 0.0f;
    for (int i = 0; i < calibrationSamples; i++) {
        sum += analogRead(FLC100_PIN) * (3.3f / 4095.0f);
        delay(10);
    }
    flcCalibrationValue = sum / calibrationSamples;
    tone(BUZZER_PIN, 2500, 200);
}

bool SensorManager::isMpuAvailable() {
    return mpuAvailable;
}
