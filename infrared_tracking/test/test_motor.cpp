#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "motor_controller.h"

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

class MotorControllerTest : public ::testing::Test {
protected:
    MockGpio gpio;
    MotorController motor{gpio, 1, 2, 3, 4};
};

TEST_F(MotorControllerTest, Forward) {
    EXPECT_CALL(gpio, setValue(1, 1));
    EXPECT_CALL(gpio, setValue(2, 0));
    EXPECT_CALL(gpio, setValue(3, 1));
    EXPECT_CALL(gpio, setValue(4, 0));
    motor.forward();
}

TEST_F(MotorControllerTest, Stop) {
    EXPECT_CALL(gpio, setValue(1, 0));
    EXPECT_CALL(gpio, setValue(2, 0));
    EXPECT_CALL(gpio, setValue(3, 0));
    EXPECT_CALL(gpio, setValue(4, 0));
    motor.stop();
}