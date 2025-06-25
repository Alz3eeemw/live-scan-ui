#include "FilterSystem.h"
#include <cmath>

KalmanFilter::KalmanFilter(float q_, float r_) : q(q_), r(r_), p(1.0f), x(0.0f), k(0.0f) {}

float KalmanFilter::update(float measurement) {
    p += q;
    k = p / (p + r);
    x += k * (measurement - x);
    p *= (1.0f - k);
    return x;
}

MovingAverageFilter::MovingAverageFilter(int size_) : size(size_), index(0), sum(0.0f), filled(false) {
    buffer = new float[size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = 0.0f;
    }
}

MovingAverageFilter::~MovingAverageFilter() {
    delete[] buffer;
}

float MovingAverageFilter::update(float value) {
    sum -= buffer[index];
    buffer[index] = value;
    sum += value;
    index = (index + 1) % size;
    if (!filled && index == 0) {
        filled = true;
    }
    return filled ? (sum / size) : (index == 0 ? sum : sum / index); // تجنب القسمة على صفر
}

LowPassFilter::LowPassFilter(float alpha_) : alpha(alpha_), prevOutput(0.0f), initialized(false) {}

float LowPassFilter::update(float input) {
    if (!initialized) {
        prevOutput = input;
        initialized = true;
    }
    prevOutput = alpha * input + (1.0f - alpha) * prevOutput;
    return prevOutput;
}

FilterSystem::FilterSystem() : kalman(0.01f, 2.0f), movingAvg(10), lowPass(0.1f) {}

float FilterSystem::processRealtime(float rawValue) {
    float v1 = movingAvg.update(rawValue);
    float v2 = lowPass.update(v1);
    return kalman.update(v2);
}

void FilterSystem::processPostScan(const float* rawData, float* filteredData, int count) {
    for (int i = 0; i < count; ++i) {
        filteredData[i] = processRealtime(rawData[i]);
    }
}
