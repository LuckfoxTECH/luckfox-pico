// io/JoystickInput.hpp
#pragma once

#include <atomic>
#include <thread>
#include <cstdint>

class JoystickInput
{
public:
    // --------------------------------------------------------
    // Lifecycle
    // --------------------------------------------------------
    static void init();     // init driver state
    void start();           // start IO task
    void stop();            // stop IO task

private:
    // --------------------------------------------------------
    // Internal task
    // --------------------------------------------------------
    void run();
    bool debounce_button(uint8_t button, uint32_t now_ms);


private:
    std::thread       thread_;
    std::atomic<bool> running_{false};

    static constexpr uint32_t DEBOUNCE_MS = 20000;
    uint32_t last_button_ts_[16]{};
};
