# Autonomous-Pathfinding-Vehicle-Project

##  Authors
Runxin Wang(2960774)
Haoze Xu(3016472)
Liyuan yu(3047156)
Yihan Wang(3047384)
Zonglin Li(2964559)

##  Project Description
This project is a real-time embedded system based on a Raspberry Pi, implementing a dual-mode control car:
1. **Automatic Line Following** using infrared sensors.
2. **Remote Control** via socket communication over TCP.

The project is built entirely in C++ using the WiringPi library and demonstrates event-driven programming through multithreading and non-blocking socket handling.

##  Features
- Line tracking with four infrared sensors
- Obstacle avoidance using rear infrared sensors
- Real-time remote command handling
- Separate modules for tracking and remote motor control

##  Hardware Used
- Raspberry Pi
- L298N Motor Driver Module
- 4 x Infrared Sensors
- DC Motors

##  How to Build and Run
```bash
# Compile the tracking system
g++ -o tracking src/tracking.cpp -lwiringPi -lpthread

# Compile the remote control server
g++ -o server src/server.cpp -lwiringPi

# Run tracking mode
sudo ./tracking

# Or run socket server (replace 8000 with your port)
sudo ./server 8000
```

##  Repository Structure
```
your-project/
├── README.md
├── LICENSE
├── src/
│   ├── tracking.cpp      # Infrared tracking module
│   └── server.cpp        # Socket-based motor control
├── images/               # (Optional) Photos or screenshots
├── video/                # (Optional) Demo video
└── docs/                 # (Optional) Technical documentation
```



##  License
MIT License
Copyright (c) 2025 [Team 25]
