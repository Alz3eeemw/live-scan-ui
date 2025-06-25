#ifndef FILTER_SYSTEM_H
#define FILTER_SYSTEM_H

class KalmanFilter {
public:
    KalmanFilter(float q_ = 0.01f, float r_ = 2.0f);
    float update(float measurement);

private:
    float q;
    float r;
    float p;
    float x;
    float k;
};

class MovingAverageFilter {
public:
    MovingAverageFilter(int size_ = 10);
    ~MovingAverageFilter();
    float update(float value);

private:
    float* buffer;
    int size;
    int index;
    float sum;
    bool filled;
};

class LowPassFilter {
public:
    LowPassFilter(float alpha_ = 0.1f);
    float update(float input);

private:
    float alpha;
    float prevOutput;
    bool initialized;
};

class FilterSystem {
public:
    FilterSystem();
    float processRealtime(float rawValue);
    void processPostScan(const float* rawData, float* filteredData, int count);

private:
    KalmanFilter       kalman;
    MovingAverageFilter movingAvg;
    LowPassFilter       lowPass;
};

#endif  // FILTER_SYSTEM_H
