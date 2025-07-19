#include "sensor_manager.h"

SensorManager::SensorManager(IGpio& gpio, int leftPin, int rightPin, int bLeftPin, int bRightPin)
    : gpio_(gpio), 
      leftPin_(leftPin), 
      rightPin_(rightPin),
      bLeftPin_(bLeftPin),
      bRightPin_(bRightPin) {
    
    gpio_.requestEvent(leftPin_, GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
    gpio_.requestEvent(rightPin_, GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
    gpio_.requestEvent(bLeftPin_, GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
    gpio_.requestEvent(bRightPin_, GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
    
    gpio_.registerEventCallback(leftPin_, [this](int pin, int value) {
        handleSensorEvent(pin, value);
    });
    gpio_.registerEventCallback(rightPin_, [this](int pin, int value) {
        handleSensorEvent(pin, value);
    });
    gpio_.registerEventCallback(bLeftPin_, [this](int pin, int value) {
        handleSensorEvent(pin, value);
    });
    gpio_.registerEventCallback(bRightPin_, [this](int pin, int value) {
        handleSensorEvent(pin, value);
    });
}

SensorManager::~SensorManager() {
    stop();
}

void SensorManager::registerCallback(SensorCallback cb) {
    mainCallback_ = cb;
}

void SensorManager::start() {
    gpio_.startEventLoop();
}

void SensorManager::stop() {
    gpio_.stopEventLoop();
}

void SensorManager::handleSensorEvent(int pin, int value) {
    if (pin == leftPin_) leftValue_ = value;
    else if (pin == rightPin_) rightValue_ = value;
    else if (pin == bLeftPin_) bLeftValue_ = value;
    else if (pin == bRightPin_) bRightValue_ = value;
    
    if (mainCallback_) {
        mainCallback_(leftValue_, rightValue_, bLeftValue_, bRightValue_);
    }
}