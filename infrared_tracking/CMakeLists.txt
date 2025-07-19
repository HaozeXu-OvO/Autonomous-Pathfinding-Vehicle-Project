project(infrared_tracking car)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)
enable_testing()

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)
find_package(Threads REQUIRED)

# Find GoogleTest
# Conservative approach: use system GTest installation
find_package(GTest REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GPIOD_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

# Link directories if needed
link_directories(
    ${GPIOD_LIBRARY_DIRS}
    ${GTEST_LIBRARY_DIRS}
)

# Compiler warnings (optional)
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# GPIO library
add_library(gpio_libgpiod
    ${SRC_DIR}/gpio_libgpiod.cpp
)
target_link_libraries(gpio_libgpiod
    ${GPIOD_LIBRARIES}
    Threads::Threads
)
target_include_directories(gpio_libgpiod PUBLIC
    ${INCLUDE_DIR}
)

# Sensor Manager
add_library(sensor_manager
    ${SRC_DIR}/sensor_manager.cpp
)
target_link_libraries(sensor_manager
    gpio_libgpiod
    Threads::Threads
)
target_include_directories(sensor_manager PUBLIC
    ${INCLUDE_DIR}
)

# Motor Controller
add_library(motor_controller
    ${SRC_DIR}/motor_controller.cpp
)
target_link_libraries(motor_controller
    Threads::Threads
)
target_include_directories(motor_controller PUBLIC
    ${INCLUDE_DIR}
)

# Tracking System
add_library(tracking_system
    ${SRC_DIR}/tracking_system.cpp
)
target_link_libraries(tracking_system
    sensor_manager
    motor_controller
    Threads::Threads
)
target_include_directories(tracking_system PUBLIC
    ${INCLUDE_DIR}
)

# Main executable
add_executable(infrared_tracking
    ${SRC_DIR}/main.cpp
)
target_link_libraries(infrared_tracking
    tracking_system
    sensor_manager
    motor_controller
    gpio_libgpiod
    ${GPIOD_LIBRARIES}
    Threads::Threads
)

# Unit tests
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

function(add_infrared_test test_name)
    add_executable(${test_name} ${TEST_DIR}/${test_name}.cpp)
    target_link_libraries(${test_name}
        ${GTEST_BOTH_LIBRARIES}
        pthread
        gpio_libgpiod
        sensor_manager
        motor_controller
        tracking_system
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Define tests
add_infrared_test(test_gpio)
add_infrared_test(test_sensor)
add_infrared_test(test_motor)
add_infrared_test(test_tracking)