#pragma once
#include <string>
#include <cstdint>

namespace signaling {

/**
 * @brief Signaling event types (AUTOSAR Sender–Receiver)
 */
enum class SigEventType : uint8_t
{
    LOCAL_SDP,
    REMOTE_SDP,
    LOCAL_ICE,
    REMOTE_ICE,
    DC_OPEN,
    TICK        // internal periodic tick
};

/**
 * @brief Generic signaling event
 */
struct SigEvent
{
    SigEventType type;
    std::string  p1;   // SDP or ICE candidate
    std::string  p2;   // ICE mid
    uint32_t     ts;   // timestamp (ms)
};

} // namespace signaling
