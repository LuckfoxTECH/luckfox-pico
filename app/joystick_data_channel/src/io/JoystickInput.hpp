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

private:
    std::thread       thread_;
    std::atomic<bool> running_{false};

};
