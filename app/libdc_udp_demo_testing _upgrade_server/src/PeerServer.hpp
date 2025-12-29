#pragma once
#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"
#include <vector>
#include <string>

class PeerServer
{
public:
    PeerServer(SignalingManager& sig, WebRTCTransport& rtc);
    void start();

private:
    SignalingManager& sig_;
    WebRTCTransport& rtc_;

    bool gotOffer_ = false;
    bool sdpDone_ = false;
    std::vector<std::pair<std::string,std::string>> iceBuffer_;
};
