#include "PeerClient.hpp"
#include <iostream>

PeerClient::PeerClient(SignalingManager& sig, WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    // ---------- Local SDP ----------
    rtc_.onLocalSdp([this](std::string sdp){
        if(!offerSent_){
            sig_.sendSdp(sdp);
            offerSent_ = true;
            std::cout << "[Client] Sent OFFER\n";
        }
    });

    // ---------- Local ICE ----------
    rtc_.onLocalIce([this](std::string cand, std::string mid){
        if(!offerSent_){
            // buffer ICE nếu OFFER chưa gửi
            static std::vector<std::pair<std::string,std::string>> iceBuffer;
            iceBuffer.emplace_back(cand, mid);
        } else {
            sig_.sendIce(cand, mid);
            std::cout << "[Client] Sent ICE: " << cand << "\n";
        }
    });

    // ---------- Remote signaling ----------
    sig_.onReceive([this](const SignalingMsg& m){
        if(m.type == SigType::SDP){
            rtc_.setRemoteAnswer(m.payload1);
            std::cout << "[Client] Got ANSWER\n";
        } else if(m.type == SigType::ICE){
            rtc_.addRemoteIce(m.payload1, m.payload2);
            std::cout << "[Client] Got remote ICE\n";
        }
    });
}

void PeerClient::start()
{
    sig_.start();
    rtc_.createOffer(); // trigger onLocalSdp -> gửi OFFER
}
