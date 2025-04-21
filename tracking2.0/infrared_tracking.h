// infrared_tracking.h
#ifndef INFRARED_TRACKING_H
#define INFRARED_TRACKING_H

#include <wiringPi.h>
#include <atomic>
#include <functional>

class GpioWrapper {
public:
    virtual void pinMode(int pin, int mode) = 0;
    virtual void digitalWrite(int pin, int value) = 0;
    virtual int digitalRead(int pin) = 0;
    virtual void pwmWrite(int pin, int value) = 0;
    virtual int wiringPiISR(int pin, int mode, void (*function)(void)) = 0;
    virtual ~GpioWrapper() = default;
};

class InfraredTracking {
public:
    InfraredTracking(GpioWrapper& gpio);
    ~InfraredTracking();
    
    void start();
    void stop();
    
    // 公开用于测试的传感器处理逻辑
    void processSensors();

private:
    GpioWrapper& gpio_;
    std::atomic<bool> running_{false};
    
    // 电机控制方法
    void runForward();
    void brake();
    void turnLeft();
    void turnRight();
    void moveBackward();
    
    // 中断服务例程
    static void sensorISR();
    static InfraredTracking* instance_;
};

#endif // INFRARED_TRACKING_H