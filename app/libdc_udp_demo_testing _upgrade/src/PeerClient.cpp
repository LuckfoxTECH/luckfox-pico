#include "PeerClient.hpp"
#include <iostream>
#include <thread>
#include <chrono>

PeerClient::PeerClient(SignalingManager& sig,
                       WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    /* ===== DataChannel ===== */

    rtc_.onDcOpen([this]{
        std::cout << "[Client] DC OPEN\n";
        running_ = true;

        std::thread([this]{
            while (running_) {
                rtc_.tick();
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(50));
            }
        }).detach();
    });

    rtc_.onDcClosed([this]{
        std::cout << "[Client] DC CLOSED\n";
        running_ = false;
    });

    rtc_.onDcMessage([](const std::string& msg){
        std::cout << "[Client] DC msg: " << msg << "\n";
    });

    /* ===== Local signaling ===== */

    rtc_.onLocalSdp([this](std::string sdp){
        std::cout << "[Client] Send OFFER\n";
        sig_.sendSdp(sdp);
    });

    rtc_.onLocalIce([this](std::string c, std::string m){
        sig_.sendIce(c, m);
    });

    /* ===== Remote signaling ===== */

    sig_.onReceive([this](const SignalingMsg& m){
        if (m.type == SigType::SDP) {
            std::cout << "[Client] Got ANSWER\n";
            rtc_.setRemoteAnswer(m.payload1);
        }
        else if (m.type == SigType::ICE) {
            rtc_.addRemoteIce(m.payload1, m.payload2);
        }
    });
}

void PeerClient::start()
{
    sig_.start();
    rtc_.createOffer();
}
