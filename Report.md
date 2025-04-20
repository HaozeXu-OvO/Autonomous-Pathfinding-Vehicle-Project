# Autonomous Pathfinding Vehicle Project

## I. Function Introduction

This project is an embedded vehicle system based on Raspberry Pi and the WiringPi library. It mainly has the following functions:

- **Automatic trajectory following**: Uses front sensors to detect ground paths, allowing the car to follow a preset trajectory.
- **Automatic stop/steering to avoid obstacles**: When the infrared sensor detects an obstacle, the car either stops or steers to avoid it.
- **WiFi remote control**: Connects to a PC via a WiFi module, enabling real-time remote control and monitoring.

## II. Code Fundamentals

### 1. Hardware abstraction and interface separation
- GPIO operations are encapsulated in a `GpioWrapper` abstract class.
- The `WiringPiWrapper` implements this interface using real WiringPi functions.
- Easily replaceable with `MockGpio` for unit testing.

### 2. Interrupt Driver Design
- `wiringPiISR` registers callbacks on the infrared sensor (INT_EDGE_BOTH).
- Upon state change, `InfraredTracking::sensorISR` is called to process sensor input with minimal delay.

### 3. Motion Control and State Management
- Controls forward, stop, turn using high/low combinations of four motor pins.
- `processSensors()` determines logic based on LEFT, RIGHT, BLEFT, BRIGHT sensors.
- Calls appropriate motion methods (`runForward`, `turnLeft`, `turnRight`, `brake`).

### 4. Non-blocking Main Loop and Signal Processing
- Main loop only uses `while (true)` with sleep.
- Core logic handled in ISR for high responsiveness.
- Handles SIGINT to safely stop the cart and release resources.

### 5. Unit Test
- Uses **Google Test** framework with `MockGpio`.
- Verifies GPIO settings, interrupt registration, and motion logic under various conditions.

## III. Practical Applications

- **Industrial and Warehouse Automation**: Can be used for auto-navigation and inspection to reduce labor cost and improve safety.
- **Home and Service Robots**: Expandable to vacuuming, delivery, patrolling, and other smart home functions.

## IV. Project Limitations

- Infrared sensors may misjudge in strong light or complex environments.
- Fixed PWM with no feedback limits speed and power.
- Acrylic chassis limits load capacity.
- Poor adaptability to non-flat or reflective surfaces.
- Short battery life limits operation time.

## V. Future Improvements

- **Multi-sensor Fusion**: Integrate ultrasonic, TOF, or LiDAR sensors for better accuracy.
- **Visual Recognition**: Add camera + image processing for SLAM and environment perception.
- **Power & Structure**: Upgrade motors, batteries, and use aluminum/carbon fiber chassis.
- **Robotic Arm & Transport**: Enable carrying or grasping objects.
- **Intelligent Scheduling & Cloud**: Support multi-vehicle coordination and remote operation via cloud platforms.
