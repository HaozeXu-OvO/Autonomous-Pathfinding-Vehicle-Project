#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "tracking_system.h"
#include "motor_controller.h"
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

class MockMotorController : public MotorController {
public:
    MockMotorController(IGpio& gpio, int in1, int in2, int in3, int in4)
        : MotorController(gpio, in1, in2, in3, in4) {}
    MOCK_METHOD(void, forward, (), (override));
    MOCK_METHOD(void, turnLeft, (), (override));
    MOCK_METHOD(void, turnRight, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};

class MockSensorManager : public SensorManager {
public:
    MockSensorManager(IGpio& gpio, int leftPin, int rightPin, int bLeftPin, int bRightPin)
        : SensorManager(gpio, leftPin, rightPin, bLeftPin, bRightPin) {}
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};

class TrackingSystemTest : public ::testing::Test {
protected:
    MockGpio gpio;
    MockMotorController motor{gpio, 1, 2, 3, 4};
    MockSensorManager sensors{gpio, 27, 26, 29, 28};
    TrackingSystem system{gpio}; // 注意：实际应用中需要依赖注入
    
    // 简化测试，直接调用回调函数
    void simulateSensorEvent(int left, int right, int bLeft, int bRight) {
        system.sensorCallback(left, right, bLeft, bRight);
    }
};

TEST_F(TrackingSystemTest, ObstacleStop) {
    EXPECT_CALL(motor, stop());
    simulateSensorEvent(0, 0, 0, 1); // 避障触发
}

TEST_F(TrackingSystemTest, Forward) {
    EXPECT_CALL(motor, forward());
    simulateSensorEvent(0, 0, 1, 1); // 无障碍，在线上
}

TEST_F(TrackingSystemTest, TurnLeft) {
    EXPECT_CALL(motor, turnLeft());
    simulateSensorEvent(1, 0, 1, 1); // 左侧检测到黑线
}