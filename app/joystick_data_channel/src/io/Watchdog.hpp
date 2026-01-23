// io/Watchdog.hpp
#pragma once

#include <atomic>

class watchdog
{
public:
    // Watchdog runnable
    static void run(std::atomic<bool>& running);
};
