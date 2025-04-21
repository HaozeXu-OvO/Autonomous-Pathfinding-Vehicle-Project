// infrared_tracking.cpp
#include "infrared_tracking.h"
#include <iostream>
#include <thread>
#include <chrono>

// 静态成员初始化
InfraredTracking* InfraredTracking::instance_ = nullptr;

InfraredTracking::InfraredTracking(GpioWrapper& gpio) 
    : gpio_(gpio) 
{
    // 初始化GPIO引脚
    gpio_.pinMode(1, OUTPUT);
    gpio_.pinMode(4, OUTPUT);
    gpio_.pinMode(5, OUTPUT);
    gpio_.pinMode(6, OUTPUT);
    
    const int BLEFT = 29;
    const int BRIGHT = 28;
    const int LEFT = 27;
    const int RIGHT = 26;
    
    gpio_.pinMode(LEFT, INPUT);
    gpio_.pinMode(RIGHT, INPUT);
    gpio_.pinMode(BLEFT, INPUT);
    gpio_.pinMode(BRIGHT, INPUT);
    
    // 设置中断
    gpio_.wiringPiISR(LEFT, INT_EDGE_BOTH, &InfraredTracking::sensorISR);
    gpio_.wiringPiISR(RIGHT, INT_EDGE_BOTH, &InfraredTracking::sensorISR);
    gpio_.wiringPiISR(BLEFT, INT_EDGE_BOTH, &InfraredTracking::sensorISR);
    gpio_.wiringPiISR(BRIGHT, INT_EDGE_BOTH, &InfraredTracking::sensorISR);
    
    instance_ = this;
}

InfraredTracking::~InfraredTracking() {
    stop();
    instance_ = nullptr;
}

void InfraredTracking::start() {
    running_ = true;
}

void InfraredTracking::stop() {
    running_ = false;
    brake();
}

void InfraredTracking::sensorISR() {
    if (instance_) {
        instance_->processSensors();
    }
}

void InfraredTracking::processSensors() {
    if (!running_) return;

    const int BLEFT = 29;
    const int BRIGHT = 28;
    const int LEFT = 27;
    const int RIGHT = 26;

    int currentLeft = gpio_.digitalRead(LEFT);
    int currentRight = gpio_.digitalRead(RIGHT);
    int currentBLeft = gpio_.digitalRead(BLEFT);
    int currentBRight = gpio_.digitalRead(BRIGHT);

    if (currentBLeft == LOW || currentBRight == LOW) {
        brake();
        return;
    }

    if (currentLeft == LOW && currentRight == LOW) {
        runForward();
    } else if (currentLeft == HIGH && currentRight == LOW) {
        turnLeft();
    } else if (currentRight == HIGH && currentLeft == LOW) {
        turnRight();
    } else {
        brake();
    }
}

// 电机控制方法实现
void InfraredTracking::runForward() {
    gpio_.digitalWrite(1, HIGH);
    gpio_.digitalWrite(4, LOW);
    gpio_.digitalWrite(5, HIGH);
    gpio_.digitalWrite(6, LOW);
    gpio_.pwmWrite(1, 1);
    gpio_.pwmWrite(5, 1);
}

void InfraredTracking::brake() {
    gpio_.digitalWrite(1, LOW);
    gpio_.digitalWrite(4, LOW);
    gpio_.digitalWrite(5, LOW);
    gpio_.digitalWrite(6, LOW);
    gpio_.pwmWrite(1, 0);
    gpio_.pwmWrite(5, 0);
}

void InfraredTracking::turnLeft() {
    gpio_.digitalWrite(1, LOW);
    gpio_.digitalWrite(4, HIGH);
    gpio_.digitalWrite(5, HIGH);
    gpio_.digitalWrite(6, LOW);
    gpio_.pwmWrite(4, 1);
    gpio_.pwmWrite(5, 1);
}

void InfraredTracking::turnRight() {
    gpio_.digitalWrite(1, HIGH);
    gpio_.digitalWrite(4, LOW);
    gpio_.digitalWrite(5, LOW);
    gpio_.digitalWrite(6, HIGH);
    gpio_.pwmWrite(1, 1);
    gpio_.pwmWrite(6, 1);
}