#include "tracking_system.h"
#include "gpio_interface.h"
#include "gpio_libgpiod.h"
#include <csignal>
#include <iostream>
#include <memory>

std::unique_ptr<TrackingSystem> tracker;

void signalHandler(int signum) {
    if (tracker) {
        tracker->stop();
    }
    std::cout << "Exiting..." << std::endl;
    exit(signum);
}

int main() {
    std::signal(SIGINT, signalHandler);
    
    try {
        auto gpio = std::make_unique<GpioLibgpiod>();
        tracker = std::make_unique<TrackingSystem>(*gpio);
        
        tracker->start();
        std::cout << "Press Ctrl+C to exit" << std::endl;
        
        while (true) {
            pause();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}