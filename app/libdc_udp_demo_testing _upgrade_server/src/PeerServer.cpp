#include "PeerServer.hpp"
#include <iostream>

PeerServer::PeerServer(SignalingManager& sig, WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    /* ---------- DataChannel ---------- */
    rtc_.onDcOpen([]
    {
        std::cout << "[Server] DC OPEN\n";
    });

    /* ---------- Local SDP ---------- */
    rtc_.onLocalSdp([this](std::string sdp)
    {
        if(!remoteSet_) {
            // chưa nhận OFFER → buffer ANSWER
            pendingAnswer_ = sdp;
            return;
        }
        if(answerSent_) return;
        answerSent_ = true;
        sig_.sendSdp(sdp);
        std::cout << "[Server] Sent ANSWER\n";
    });

    /* ---------- Local ICE ---------- */
    rtc_.onLocalIce([this](std::string cand, std::string mid)
    {
        sig_.sendIce(cand, mid);
    });

    /* ---------- Receive signaling ---------- */
    sig_.onReceive([this](const SignalingMsg& m)
    {
        if(m.type == SigType::SDP)
        {
            if(remoteSet_) {
                std::cout << "[Server] Ignore duplicate OFFER\n";
                return;
            }

            rtc_.setRemoteOffer(m.payload1);
            remoteSet_ = true;

            std::cout << "[Server] Got OFFER\n";

            // flush buffered ICE
            for(auto& c : iceBuffer_)
                rtc_.addRemoteIce(c.first, c.second);
            iceBuffer_.clear();

            // gửi ANSWER đã buffer hoặc tạo mới
            if(!pendingAnswer_.empty())
            {
                sig_.sendSdp(pendingAnswer_);
                answerSent_ = true;
                pendingAnswer_.clear();
                std::cout << "[Server] Sent ANSWER\n";
            }
            else
            {
                rtc_.createAnswer();
            }
        }
        else if(m.type == SigType::ICE)
        {
            if(!remoteSet_)
            {
                iceBuffer_.emplace_back(m.payload1, m.payload2);
            }
            else
            {
                rtc_.addRemoteIce(m.payload1, m.payload2);
                std::cout << "[Server] Got remote ICE: " << m.payload1 << "\n";

            }
        }
    });
}

void PeerServer::start()
{
    sig_.start();
}
