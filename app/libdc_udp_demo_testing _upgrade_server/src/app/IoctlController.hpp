// io/IoctlController.hpp
#pragma once

#include <atomic>
#include <thread>
#include <cstdint>

class IoctlController
{
public:
    // --------------------------------------------------------
    // Lifecycle
    // --------------------------------------------------------
    IoctlController();
    ~IoctlController();

    void start();   // start ioctl task
    void stop();    // stop ioctl task

private:
    // --------------------------------------------------------
    // Internal task
    // --------------------------------------------------------
    void run();                 // main loop
    void applyControl();        // apply control via ioctl

private:
    // --------------------------------------------------------
    // State
    // --------------------------------------------------------
    int m_fd{-1};               // device file descriptor

    std::thread       thread_;
    std::atomic<bool> running_{false};
};
