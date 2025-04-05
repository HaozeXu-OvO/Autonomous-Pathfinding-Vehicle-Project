#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

// 定义红外寻迹类
class InfraredTracking {
private:
    static const int BLEFT = 29;
    static const int BRIGHT = 28;
    static const int LEFT = 27;
    static const int RIGHT = 26;
    //static const int PWMA = 2;
    //static const int PWMB = 3;
    
    std::atomic<bool> running{true};
    std::atomic<int> lastLeftState{LOW};
    std::atomic<int> lastRightState{LOW};
    
    // 前进
    void run() {
        digitalWrite(1, HIGH);  // IN1为HIGH，左边电机正转
        digitalWrite(4, LOW);   // IN2为LOW
        digitalWrite(5, HIGH);  // IN3为HIGH，右边电机正转
        digitalWrite(6, LOW);   // IN4为LOW
        pwmWrite(1, 1);        // 左电机速度，极低
        pwmWrite(4, 0);
        pwmWrite(5, 1);        // 右电机速度，极低
        pwmWrite(6, 0);
    }

    // 刹车停止
    void brake() 
    {
        digitalWrite(1, LOW);   // IN1为LOW
        digitalWrite(4, LOW);   // IN2为LOW
        digitalWrite(5, LOW);   // IN3为LOW
        digitalWrite(6, LOW);   // IN4为LOW
        pwmWrite(1, 0);        // 左电机停止
        pwmWrite(4, 0);
        pwmWrite(5, 0);        // 右电机停止
        pwmWrite(6, 0);        // 右电机停止
    }

    // 左转
    void left() 
    {
        digitalWrite(1, LOW);   // IN1为LOW
        digitalWrite(4, HIGH);  // IN2为HIGH，左边电机反转
        digitalWrite(5, HIGH);  // IN3为HIGH，右边电机正转
        digitalWrite(6, LOW);   // IN4为LOW
        pwmWrite(1, 0);
        pwmWrite(4, 1);        // 左电机速度，极低
        pwmWrite(5, 1);        // 右电机速度，极低
        pwmWrite(6, 0);
    }

    // 右转
    void right() {
        digitalWrite(1, HIGH);  // IN1为HIGH，左边电机正转
        digitalWrite(4, LOW);   // IN2为LOW
        digitalWrite(5, LOW);   // IN3为LOW
        digitalWrite(6, HIGH);  // IN4为HIGH，右边电机反转
        pwmWrite(1, 1);        // 左电机速度，极低
        pwmWrite(4, 0);
        pwmWrite(5, 0);
        pwmWrite(6, 1);        // 右电机速度，极低
    }

    // 后退
    void back() 
    {
        digitalWrite(1, LOW);   // IN1为LOW
        digitalWrite(4, HIGH);  // IN2为HIGH，左边电机反转
        digitalWrite(5, LOW);   // IN3为LOW
        digitalWrite(6, HIGH);  // IN4为HIGH，右边电机反转
        pwmWrite(1, 0);
        pwmWrite(4, 1);        // 左电机速度，极低
        pwmWrite(5, 0);
        pwmWrite(6, 1);        // 右电机速度，极低
    }

    // 传感器状态变化处理函数
    void handleSensorChange() {
        int currentLeft = digitalRead(LEFT);
        int currentRight = digitalRead(RIGHT);
        int currentBLeft = digitalRead(BLEFT);
        int currentBRight = digitalRead(BRIGHT);

        // 打印传感器状态，用于调试
        std::cout << "Left: " << currentLeft << " Right: " << currentRight << " BLeft: " << currentBLeft << " BRight: " << currentBRight << std::endl;
        if(currentBLeft == LOW || currentBRight == LOW)
        {
            brake();
            std::cout << "AVOID" << std::endl;
        }
        else
        {
            // 只在状态发生变化时处理
           
            if (currentLeft == LOW && currentRight == LOW) 
                {
                // 两个传感器都在白线上，直行
                std::cout << "GO" << std::endl;
                run();
                 } 
            else if (currentLeft == HIGH && currentRight == LOW) 
                {
                // 左传感器检测到黑线，需要左转
                std::cout << "LEFT" << std::endl;
                left();
                 } 
            else if (currentRight == HIGH && currentLeft == LOW) 
                {
                // 右传感器检测到黑线，需要右转
                std::cout << "RIGHT" << std::endl;
                right();
                }
            else if (currentLeft == HIGH && currentRight == HIGH) 
                {
                // 两个传感器都检测到黑线，停止
                std::cout << "STOP" << std::endl;
                brake();
                }
            
            lastLeftState = currentLeft;
            lastRightState = currentRight;
            
        }
    }

    // 定时器回调函数
    void timerCallback() {
        while (running) {
            handleSensorChange();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

public:
    // 构造函数，初始化硬件
    InfraredTracking() {
        if (wiringPiSetup() == -1) {
            std::cout << "WiringPi setup failed!" << std::endl;
            return;
        }
        
        // 设置引脚模式
        pinMode(1, OUTPUT);     // IN1
        pinMode(4, OUTPUT);     // IN2
        pinMode(5, OUTPUT);     // IN3
        pinMode(6, OUTPUT);     // IN4
        
        // 设置PWM模式
        pwmSetMode(PWM_MODE_MS);
        pwmSetClock(40960);     // 大幅增加PWM时钟，降低频率
        pwmSetRange(10);        // 大幅减小PWM范围，降低最大速度
        
        // 设置传感器引脚为输入
        pinMode(LEFT, INPUT);
        pinMode(RIGHT, INPUT);
        pinMode(BLEFT, INPUT); 
        pinMode(BRIGHT, INPUT);

        std::cout << "Hardware initialized successfully!" << std::endl;
    }

    // 开始寻迹
    void startTracking() {
        std::cout << "Starting tracking..." << std::endl;
        
        // 创建定时器线程
        std::thread timerThread(&InfraredTracking::timerCallback, this);
        
        // 等待用户输入来停止
        std::cout << "Press Enter to stop..." << std::endl;
        std::cin.get();
        
        // 停止所有操作
        running = false;
        timerThread.join();
        brake();
    }
    
    // 析构函数
    ~InfraredTracking() {
        running = false;
        brake();
    }
};

int main(int argc, char *argv[]) {
    InfraredTracking tracker;
    tracker.startTracking();
    return 0;
} 