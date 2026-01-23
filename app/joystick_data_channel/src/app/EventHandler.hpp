// app/EventHandler.hpp
#pragma once

#include <atomic>

class EventHandler
{
public:
    static void run(std::atomic<bool>& running);
};
