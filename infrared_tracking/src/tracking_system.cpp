#include "tracking_system.h"
#include <iostream>

constexpr int LEFT_PIN = 27;
constexpr int RIGHT_PIN = 26;
constexpr int BLEFT_PIN = 29;
constexpr int BRIGHT_PIN = 28;
constexpr int IN1 = 1;
constexpr int IN2 = 4;
constexpr int IN3 = 5;
constexpr int IN4 = 6;

TrackingSystem::TrackingSystem(IGpio& gpio)
    : gpio_(gpio),
      motor_(gpio, IN1, IN2, IN3, IN4),
      sensors_(gpio, LEFT_PIN, RIGHT_PIN, BLEFT_PIN, BRIGHT_PIN) {
    
    sensors_.registerCallback([this](int l, int r, int bl, int br) {
        sensorCallback(l, r, bl, br);
    });
}

TrackingSystem::~TrackingSystem() {
    stop();
}

void TrackingSystem::start() {
    sensors_.start();
    std::cout << "Tracking system started" << std::endl;
}

void TrackingSystem::stop() {
    sensors_.stop();
    motor_.stop();
    std::cout << "Tracking system stopped" << std::endl;
}

void TrackingSystem::sensorCallback(int left, int right, int bLeft, int bRight) {
    std::cout << "Sensors - Left: " << left 
              << " Right: " << right
              << " BLeft: " << bLeft
              << " BRight: " << bRight << std::endl;
    
    if (bLeft == 0 || bRight == 0) {
        std::cout << "Obstacle detected: STOP" << std::endl;
        motor_.stop();
        return;
    }
    
    if (left == 0 && right == 0) {
        std::cout << "GO" << std::endl;
        motor_.forward();
    } else if (left == 1 && right == 0) {
        std::cout << "LEFT" << std::endl;
        motor_.turnLeft();
    } else if (right == 1 && left == 0) {
        std::cout << "RIGHT" << std::endl;
        motor_.turnRight();
    } else {
        std::cout << "STOP" << std::endl;
        motor_.stop();
    }
}