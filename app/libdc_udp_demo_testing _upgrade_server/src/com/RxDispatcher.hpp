// com/RxDispatcher.hpp
#pragma once

#include "Frame.hpp"
#include <iomanip>
#include <iostream>

/*
 * RxDispatcher
 * -------------
 * COM RX entry point
 * - Decode Frame
 * - Update VehicleState
 * - Push COM_RX event
 */
class RxDispatcher
{
public:
    static void dispatch(const Frame& frame);
};
