#ifndef GPIO_LIBGPIOD_H
#define GPIO_LIBGPIOD_H

#include "gpio_interface.h"
#include <gpiod.h>
#include <thread>
#include <map>
#include <atomic>

class GpioLibgpiod : public IGpio {
public:
    GpioLibgpiod();
    ~GpioLibgpiod() override;
    bool requestInput(int pin) override;
    bool requestOutput(int pin) override;
    bool requestEvent(int pin, int event_flags) override;
    void setValue(int pin, int value) override;
    int getValue(int pin) override;
    void registerEventCallback(int pin, EventCallback cb) override;
    void startEventLoop() override;
    void stopEventLoop() override;
private:
    bool requestLine(int pin, int direction);
    void eventLoop();

    gpiod_chip* chip_ = nullptr;
    std::map<int, gpiod_line*> lines_;
    std::map<int, EventCallback> callbacks_;
    std::map<int, int> event_fds_;
    std::thread event_thread_;
    std::atomic<bool> running_{ false };
};

#endif // GPIO_LIBGPIOD_H