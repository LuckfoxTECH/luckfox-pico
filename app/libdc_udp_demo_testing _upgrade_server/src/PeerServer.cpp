#include "PeerServer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

PeerServer::PeerServer(SignalingManager& sig,
                       WebRTCTransport& rtc)
    : sig_(sig),
      rtc_(rtc),
      gotOffer_(false),
      sdpDone_(false)
{
    /* ===== DATA CHANNEL ===== */
    rtc_.onDcOpen([this]{
        std::cout << "[Server] DC OPEN\n";

        std::thread([this]{
            int i = 0;
            while (true) {
                rtc_.sendMessage(
                    "Hello client #" + std::to_string(i++));
                std::this_thread::sleep_for(
                    std::chrono::seconds(1));
            }
        }).detach();
    });

    rtc_.onDcMessage([](const std::string& msg){
        std::cout << "[Server] DC received: " << msg << "\n";
    });

    /* ===== LOCAL SDP (ANSWER) ===== */
    rtc_.onLocalSdp([this](std::string sdp){
        if (sdpDone_) {
            std::cout << "[Server] Ignore duplicate ANSWER\n";
            return;
        }

        sig_.sendSdp(sdp);
        sdpDone_ = true;

        std::cout << "[Server] Sent ANSWER\n";
    });

    /* ===== LOCAL ICE ===== */
    rtc_.onLocalIce([this](std::string cand, std::string mid){
        sig_.sendIce(cand, mid);
        std::cout << "[Server] Sent ICE: " << cand << "\n";
    });

    /* ===== SIGNALING RECEIVE ===== */
    sig_.onReceive([this](const SignalingMsg& m){
        if (m.type == SigType::SDP) {

            if (gotOffer_) {
                std::cout << "[Server] Ignore duplicate OFFER\n";
                return;
            }

            std::cout << "[Server] Got OFFER\n";
            gotOffer_ = true;

            rtc_.setRemoteOffer(m.payload1);
            rtc_.createAnswer(); 

        } else if (m.type == SigType::ICE) {

            rtc_.addRemoteIce(m.payload1, m.payload2);
            std::cout << "[Server] Got remote ICE\n";
        }
    });
}

void PeerServer::start()
{
    sig_.start();
}
