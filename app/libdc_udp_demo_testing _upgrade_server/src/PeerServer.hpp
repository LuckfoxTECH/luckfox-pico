#pragma once
#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"
#include <vector>
#include <string>
#include <utility>

class PeerServer
{
public:
    PeerServer(SignalingManager& sig, WebRTCTransport& rtc);
    void start();

private:
    SignalingManager& sig_;
    WebRTCTransport& rtc_;

    bool remoteSet_  = false;
    bool answerSent_ = false;

    std::string pendingAnswer_; // buffer ANSWER nếu onLocalSdp trigger sớm
    std::vector<std::pair<std::string,std::string>> iceBuffer_;
};
