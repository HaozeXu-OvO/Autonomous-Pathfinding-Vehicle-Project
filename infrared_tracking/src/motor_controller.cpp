#include "motor_controller.h"

MotorController::MotorController(IGpio& gpio, int in1, int in2, int in3, int in4)
    : gpio_(gpio), in1_(in1), in2_(in2), in3_(in3), in4_(in4) {
    
    gpio_.requestOutput(in1_);
    gpio_.requestOutput(in2_);
    gpio_.requestOutput(in3_);
    gpio_.requestOutput(in4_);
    
    stop();
}

void MotorController::forward() {
    gpio_.setValue(in1_, 1);
    gpio_.setValue(in2_, 0);
    gpio_.setValue(in3_, 1);
    gpio_.setValue(in4_, 0);
}

void MotorController::backward() {
    gpio_.setValue(in1_, 0);
    gpio_.setValue(in2_, 1);
    gpio_.setValue(in3_, 0);
    gpio_.setValue(in4_, 1);
}

void MotorController::turnLeft() {
    gpio_.setValue(in1_, 0);
    gpio_.setValue(in2_, 1);
    gpio_.setValue(in3_, 1);
    gpio_.setValue(in4_, 0);
}

void MotorController::turnRight() {
    gpio_.setValue(in1_, 1);
    gpio_.setValue(in2_, 0);
    gpio_.setValue(in3_, 0);
    gpio_.setValue(in4_, 1);
}

void MotorController::stop() {
    gpio_.setValue(in1_, 0);
    gpio_.setValue(in2_, 0);
    gpio_.setValue(in3_, 0);
    gpio_.setValue(in4_, 0);
}