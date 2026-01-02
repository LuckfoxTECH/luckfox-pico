#pragma once

#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"
#include <atomic>

/*
 * PeerClient
 * ----------
 * Glue layer:
 *   - Kết nối SignalingManager (UDP / WS / TCP)
 *   - Với WebRTCTransport (libdatachannel)
 *
 * main.cpp chỉ cần:
 *   PeerClient client(sig, rtc);
 *   client.start();
 */
class PeerClient
{
public:
    PeerClient(SignalingManager& sig,
               WebRTCTransport& rtc);

    void start();

private:
    /* ===== External components ===== */
    SignalingManager& sig_;
    WebRTCTransport& rtc_;

    /* ===== Runtime state ===== */
    std::atomic<bool> running_{false};
};
