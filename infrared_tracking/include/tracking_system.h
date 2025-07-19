#ifndef TRACKING_SYSTEM_H
#define TRACKING_SYSTEM_H

#include "motor_controller.h"
#include "sensor_manager.h"

class TrackingSystem {
public:
    TrackingSystem(IGpio& gpio);
    ~TrackingSystem();
    
    void start();
    void stop();

private:
    void sensorCallback(int left, int right, int bLeft, int bRight);
    
    IGpio& gpio_;
    MotorController motor_;
    SensorManager sensors_;
};

#endif // TRACKING_SYSTEM_H