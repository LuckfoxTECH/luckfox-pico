#include "PeerServer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

PeerServer::PeerServer(SignalingManager& sig,
                       WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    rtc_.onDcOpen([this]{
        std::cout << "[Server] DC OPEN\n";
        running_ = true;

        std::thread([this]{
            int i = 0;
            while (running_) {
                rtc_.tick();
                rtc_.sendMessage(
                    "Hello client #" + std::to_string(i++));
                std::this_thread::sleep_for(
                    std::chrono::seconds(1));
            }
        }).detach();
    });

    rtc_.onDcClosed([this]{
        std::cout << "[Server] DC CLOSED\n";
        running_ = false;
    });

    rtc_.onDcMessage([](const std::string& msg){
        std::cout << "[Server] DC msg: " << msg << "\n";
    });

    rtc_.onLocalSdp([this](std::string sdp){
        sig_.sendSdp(sdp);
    });

    rtc_.onLocalIce([this](std::string c, std::string m){
        sig_.sendIce(c, m);
    });

    sig_.onReceive([this](const SignalingMsg& m){
        if (m.type == SigType::SDP)
            rtc_.setRemoteOffer(m.payload1);
        else if (m.type == SigType::ICE)
            rtc_.addRemoteIce(m.payload1, m.payload2);
    });
}

void PeerServer::start()
{
    sig_.start();
}
