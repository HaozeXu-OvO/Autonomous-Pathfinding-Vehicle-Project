# Autonomous-Pathfinding-Vehicle-Project

![efef5132e0ed9fa067b3fb223c5d83f](https://github.com/user-attachments/assets/e7e53a68-35a7-4ce6-9f35-9f7b7ff200f1)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-red)](https://www.raspberrypi.com/)
[![Language](https://img.shields.io/badge/Language-C++-blue)](https://isocpp.org/)
[![Bilibili](https://img.shields.io/badge/Bilibili-Video-ff69b4)](https://b23.tv/2XWn9GX)
[![YouTube](https://img.shields.io/badge/YouTube-Video%20Demo-ff0000)](https://youtu.be/0SxoRG_W2z0?feature=shared)

##  Authors
Runxin Wang(2960774)
Haoze Xu(3016472)
Liyuan yu(3047156)
Yihan Wang(3047384)
Zonglin Li(2964559)

## Team Roles and Responsibilities

| Name               | Student ID | Responsibilities                                                                |
|--------------------|------------|---------------------------------------------------------------------------------|
| Runxin Wang        | 2960774    | Responsible for the design of the car and the display of the video.             |
| Haoze Xu           | 3016472    | Program editor, car debugging, team leader, task assignment.                    |
| Liyuan Yu          | 3047156    | Participated in programming of the car and editing the GitHub repository.       |
| Yihan Wang         | 3047384    | Program editing, car debugging, and function testing.                           |
| Zonglin Li         | 2964559    | Report writing, device procurement, PPT content creation, and video recording.  |


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
Copyright (c) 2025 Team 25
