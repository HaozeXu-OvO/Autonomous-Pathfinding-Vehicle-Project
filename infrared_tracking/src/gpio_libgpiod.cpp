#include "gpio_interface.h"
#include "gpio_libgpiod.h"
#include <gpiod.h>
#include <thread>
#include <atomic>
#include <map>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <system_error>
#include <cstring>

GpioLibgpiod::GpioLibgpiod() : running_(false) {
    chip_ = gpiod_chip_open("/dev/gpiochip0");
    if (!chip_) throw std::runtime_error("Failed to open GPIO chip");
}

GpioLibgpiod::~GpioLibgpiod() {
    stopEventLoop();
    for (auto& [pin, line] : lines_) gpiod_line_release(line);
    if (chip_) gpiod_chip_close(chip_);
}

bool GpioLibgpiod::requestInput(int pin) { return requestLine(pin, GPIOD_LINE_REQUEST_DIRECTION_INPUT); }
bool GpioLibgpiod::requestOutput(int pin) { return requestLine(pin, GPIOD_LINE_REQUEST_DIRECTION_OUTPUT); }
bool GpioLibgpiod::requestEvent(int pin, int event_flags) {
    gpiod_line* line = gpiod_chip_get_line(chip_, pin);
    if (!line) return false;
    struct gpiod_line_request_config cfg { "infrared_tracking", event_flags, 0 };
    if (gpiod_line_request(line, &cfg, 0) < 0) return false;
    int fd = gpiod_line_event_get_fd(line);
    if (fd < 0) { gpiod_line_release(line); return false; }
    lines_[pin] = line; event_fds_[fd] = pin;
    return true;
}

void GpioLibgpiod::setValue(int pin, int value) {
    auto it = lines_.find(pin);
    if (it != lines_.end()) gpiod_line_set_value(it->second, value);
}

int GpioLibgpiod::getValue(int pin) {
    auto it = lines_.find(pin);
    return it != lines_.end() ? gpiod_line_get_value(it->second) : -1;
}

void GpioLibgpiod::registerEventCallback(int pin, EventCallback cb) {
    callbacks_[pin] = cb;
}

void GpioLibgpiod::startEventLoop() {
    if (running_) return;
    running_ = true;
    event_thread_ = std::thread([this] { eventLoop(); });
}

void GpioLibgpiod::stopEventLoop() {
    running_ = false;
    if (event_thread_.joinable()) event_thread_.join();
}

bool GpioLibgpiod::requestLine(int pin, int dir) {
    gpiod_line* line = gpiod_chip_get_line(chip_, pin);
    if (!line) return false;
    struct gpiod_line_request_config cfg { "infrared_tracking", dir, 0 };
    if (gpiod_line_request(line, &cfg, 0) < 0) return false;
    lines_[pin] = line; return true;
}

void GpioLibgpiod::eventLoop() {
    while (running_) {
        std::vector<pollfd> fds;
        for (auto& [fd, pin] : event_fds_) fds.push_back({ fd, POLLIN,0 });
        if (fds.empty()) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); continue; }
        int ret = poll(fds.data(), fds.size(), 100);
        if (ret < 0) { if (errno == EINTR) continue; throw std::system_error(errno, std::generic_category(), "poll failed"); }
        if (ret == 0) continue;
        for (auto& pfd : fds) {
            if (pfd.revents & POLLIN) {
                int pin = event_fds_[pfd.fd];
                gpiod_line_event ev;
                if (gpiod_line_event_read_fd(pfd.fd, &ev) == 0) {
                    int value = (ev.event_type == GPIOD_LINE_EVENT_RISING_EDGE) ? 1 : 0;
                    if (callbacks_.count(pin)) callbacks_[pin](pin, value);
                }
            }
        }
    }
}