#include "PeerClient.hpp"
#include <iostream>

PeerClient::PeerClient(SignalingManager& sig, WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    /* ---------- Send OFFER ---------- */
    rtc_.onLocalSdp([&](std::string sdp) {
        sig_.sendSdp(sdp);
        std::cout << "[Client] Sent OFFER\n";
    });

    /* ---------- Send local ICE ---------- */
    rtc_.onLocalIce([&](std::string cand, std::string mid) {
        sig_.sendIce(cand, mid);
        std::cout << "[Client] Sent ICE: " << cand << "\n";
    });

    /* ---------- DC open ---------- */
    rtc_.onDcOpen([]() {
        std::cout << "[Client] DC OPEN\n";
    });

    /* ---------- Receive signaling ---------- */
    sig_.onReceive([&](const SignalingMsg& m) {
        if(m.type == SigType::SDP) {
            rtc_.setRemoteAnswer(m.payload1);
            std::cout << "[Client] Got ANSWER\n";
        }
        else if(m.type == SigType::ICE) {
            rtc_.addRemoteIce(m.payload1, m.payload2);
            std::cout << "[Client] Got remote ICE\n";
        }
    });
}

void PeerClient::start()
{
    sig_.start();
    rtc_.createOffer();
}
