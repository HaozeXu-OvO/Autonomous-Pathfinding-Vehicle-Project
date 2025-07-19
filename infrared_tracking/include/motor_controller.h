#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "gpio_interface.h"

class MotorController {
public:
    MotorController(IGpio& gpio, int in1, int in2, int in3, int in4);
    
    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    void stop();

private:
    IGpio& gpio_;
    const int in1_, in2_, in3_, in4_;
};

#endif // MOTOR_CONTROLLER_H