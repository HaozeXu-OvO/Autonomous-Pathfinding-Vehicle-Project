// main.cpp
#include "infrared_tracking.h"
#include <wiringPi.h>
#include <iostream>
#include <csignal>

class WiringPiWrapper : public GpioWrapper {
public:
    void pinMode(int pin, int mode) override {
        ::pinMode(pin, mode);
    }
    
    void digitalWrite(int pin, int value) override {
        ::digitalWrite(pin, value);
    }
    
    int digitalRead(int pin) override {
        return ::digitalRead(pin);
    }
    
    void pwmWrite(int pin, int value) override {
        ::pwmWrite(pin, value);
    }
    
    int wiringPiISR(int pin, int mode, void (*function)(void)) override {
        return ::wiringPiISR(pin, mode, function);
    }
};

InfraredTracking* tracker = nullptr;

void signalHandler(int signal) {
    if (tracker) {
        tracker->stop();
    }
    exit(signal);
}

int main() {
    if (wiringPiSetup() == -1) {
        std::cerr << "WiringPi initialization failed!" << std::endl;
        return 1;
    }

    WiringPiWrapper gpio;
    InfraredTracking tracking(gpio);
    tracker = &tracking;
    
    std::signal(SIGINT, signalHandler);
    
    tracking.start();
    std::cout << "Tracking started. Press Ctrl+C to exit." << std::endl;
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}