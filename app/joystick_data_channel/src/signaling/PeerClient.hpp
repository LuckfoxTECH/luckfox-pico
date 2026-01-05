#pragma once

#include "SignalingManager.hpp"
#include "transport/WebRTCTransport.hpp"

/*
 * PeerClient
 * ----------
 * Signaling glue layer:
 *   - Kết nối SignalingManager (UDP / WS / TCP)
 *   - Với WebRTCTransport (libdatachannel)
 *
 * Trách nhiệm:
 *   - Gửi SDP / ICE local
 *   - Nhận SDP / ICE remote
 *
 * KHÔNG:
 *   - tick()
 *   - thread
 *   - keepalive
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
};
