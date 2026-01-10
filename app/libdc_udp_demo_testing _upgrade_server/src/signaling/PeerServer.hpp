#pragma once

#include "SignalingManager.hpp"
#include "transport/WebRTCTransport.hpp"
#include <atomic>

class PeerServer {
public:
    PeerServer(SignalingManager& sig,
               WebRTCTransport& rtc);
    void start();

private:
    SignalingManager& sig_;
    WebRTCTransport& rtc_;
    std::atomic<bool> running_{false};
};
