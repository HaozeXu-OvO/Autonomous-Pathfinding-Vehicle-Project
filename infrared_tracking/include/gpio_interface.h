#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include <functional>
#include <vector>
#include <memory>

class IGpio {
public:
    using EventCallback = std::function<void(int, int)>; // pin, value
    
    virtual ~IGpio() = default;
    
    virtual bool requestInput(int pin) = 0;
    virtual bool requestOutput(int pin) = 0;
    virtual bool requestEvent(int pin, int event_flags) = 0;
    virtual void setValue(int pin, int value) = 0;
    virtual int getValue(int pin) = 0;
    virtual void registerEventCallback(int pin, EventCallback cb) = 0;
    virtual void startEventLoop() = 0;
    virtual void stopEventLoop() = 0;
};

#endif // GPIO_INTERFACE_H