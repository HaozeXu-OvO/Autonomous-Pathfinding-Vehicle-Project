#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "gpio_interface.h"
#include <functional>
#include <atomic>

class SensorManager {
public:
    using SensorCallback = std::function<void(int, int, int, int)>;
    
    SensorManager(IGpio& gpio, int leftPin, int rightPin, int bLeftPin, int bRightPin);
    ~SensorManager();
    
    void registerCallback(SensorCallback cb);
    void start();
    void stop();
    
private:
    void handleSensorEvent(int pin, int value);
    
    IGpio& gpio_;
    const int leftPin_, rightPin_, bLeftPin_, bRightPin_;
    SensorCallback mainCallback_;
    std::atomic<int> leftValue_{0};
    std::atomic<int> rightValue_{0};
    std::atomic<int> bLeftValue_{0};
    std::atomic<int> bRightValue_{0};
};

#endif // SENSOR_MANAGER_H