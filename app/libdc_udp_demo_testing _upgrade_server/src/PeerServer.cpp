#include "PeerServer.hpp"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

PeerServer::PeerServer(SignalingManager& sig,
                       WebRTCTransport& rtc)
    : sig_(sig),
      rtc_(rtc),
      running_(false)
{
    /* ================================
     *  DATA CHANNEL
     * ================================ */

    rtc_.onDcOpen([this]{
        std::cout << "[Server] DC OPEN\n";

        running_ = true;

        /* sender thread */
        std::thread([this]{
            int i = 0;
            while (running_) {
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

    /* ================================
     *  LOCAL SDP (ANSWER)
     * ================================ */

    rtc_.onLocalSdp([this](std::string sdp){
        std::cout << "[Server] Send ANSWER\n";
        sig_.sendSdp(sdp);
    });

    /* ================================
     *  LOCAL ICE
     * ================================ */

    rtc_.onLocalIce([this](std::string cand, std::string mid){
        sig_.sendIce(cand, mid);
        std::cout << "[Server] Sent ICE\n";
    });

    /* ================================
     *  SIGNALING RECEIVE
     * ================================ */

    sig_.onReceive([this](const SignalingMsg& m){

        if (m.type == SigType::SDP) {

            std::cout << "[Server] Got OFFER → reset / new session\n";

            rtc_.setRemoteOffer(m.payload1);

        } else if (m.type == SigType::ICE) {

            rtc_.addRemoteIce(m.payload1, m.payload2);
        }
    });
}

void PeerServer::start()
{
    sig_.start();
}
