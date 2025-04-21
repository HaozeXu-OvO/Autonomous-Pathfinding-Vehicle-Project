// test_infrared_tracking.cpp
#include "infrared_tracking.h"
#include <gtest/gtest.h>

class MockGpio : public GpioWrapper {
public:
    std::map<int, int> pinModes;
    std::map<int, int> pinValues;
    std::map<int, int> pwmValues;
    int isrCount = 0;
    
    void pinMode(int pin, int mode) override {
        pinModes[pin] = mode;
    }
    
    void digitalWrite(int pin, int value) override {
        pinValues[pin] = value;
    }
    
    int digitalRead(int pin) override {
        return pinValues[pin];
    }
    
    void pwmWrite(int pin, int value) override {
        pwmValues[pin] = value;
    }
    
    int wiringPiISR(int pin, int mode, void (*function)(void)) override {
        isrCount++;
        return 0;
    }
};

class InfraredTrackingTest : public ::testing::Test {
protected:
    MockGpio mockGpio;
    InfraredTracking tracker{mockGpio};
};

TEST_F(InfraredTrackingTest, InitializationTest) {
    EXPECT_EQ(mockGpio.pinModes[1], OUTPUT);
    EXPECT_EQ(mockGpio.pinModes[27], INPUT);
    EXPECT_GE(mockGpio.isrCount, 4);
}

TEST_F(InfraredTrackingTest, SensorLogicTest) {
    tracker.start();
    
    // Test forward movement
    mockGpio.pinValues[27] = LOW;  // LEFT
    mockGpio.pinValues[26] = LOW;  // RIGHT
    tracker.processSensors();
    
    EXPECT_EQ(mockGpio.pwmValues[1], 1);
    EXPECT_EQ(mockGpio.pwmValues[5], 1);
    
    // Test left turn
    mockGpio.pinValues[27] = HIGH;
    mockGpio.pinValues[26] = LOW;
    tracker.processSensors();
    
    EXPECT_EQ(mockGpio.pwmValues[4], 1);
    EXPECT_EQ(mockGpio.pwmValues[5], 1);
    
    // Test stop condition
    mockGpio.pinValues[27] = HIGH;
    mockGpio.pinValues[26] = HIGH;
    tracker.processSensors();
    
    EXPECT_EQ(mockGpio.pwmValues[1], 0);
    EXPECT_EQ(mockGpio.pwmValues[5], 0);
}