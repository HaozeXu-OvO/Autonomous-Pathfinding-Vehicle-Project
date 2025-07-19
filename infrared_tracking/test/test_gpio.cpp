#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gpio_interface.h"

class MockGpio : public IGpio {
public:
    MOCK_METHOD(bool, requestInput, (int pin), (override));
    MOCK_METHOD(bool, requestOutput, (int pin), (override));
    MOCK_METHOD(bool, requestEvent, (int pin, int event_flags), (override));
    MOCK_METHOD(void, setValue, (int pin, int value), (override));
    MOCK_METHOD(int, getValue, (int pin), (override));
    MOCK_METHOD(void, registerEventCallback, (int pin, EventCallback cb), (override));
    MOCK_METHOD(void, startEventLoop, (), (override));
    MOCK_METHOD(void, stopEventLoop, (), (override));
};

TEST(GpioInterfaceTest, RequestEvent) {
    MockGpio gpio;
    EXPECT_CALL(gpio, requestEvent(1, testing::_)).WillOnce(testing::Return(true));
    EXPECT_TRUE(gpio.requestEvent(1, 0));
}

// 更多GPIO接口测试...