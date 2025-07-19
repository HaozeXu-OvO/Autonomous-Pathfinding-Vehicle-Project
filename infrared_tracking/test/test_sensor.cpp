#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "sensor_manager.h"

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

class SensorManagerTest : public ::testing::Test {
protected:
    MockGpio gpio;
    SensorManager sensor{gpio, 1, 2, 3, 4};
};

TEST_F(SensorManagerTest, EventCallback) {
    testing::MockFunction<void(int, int, int, int)> callback;
    sensor.registerCallback(callback.AsStdFunction());
    
    EXPECT_CALL(callback, Call(1, 0, 0, 0));
    sensor.handleSensorEvent(1, 1);
    
    EXPECT_CALL(callback, Call(1, 1, 0, 0));
    sensor.handleSensorEvent(2, 1);
}