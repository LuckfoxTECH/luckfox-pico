#pragma once

#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"

/*
 * PeerClient
 * ----------
 * Lớp glue:
 *   - Nối SignalingManager (UDP + FSM)
 *   - Với WebRTCTransport (libdatachannel)
 *
 * main.cpp chỉ cần gọi:
 *   PeerClient client(...);
 *   client.start();
 */
class PeerClient {
public:
    PeerClient(SignalingManager& sig,
               WebRTCTransport& rtc);

    void start();

private:
    SignalingManager& sig_;
    WebRTCTransport& rtc_;
};
