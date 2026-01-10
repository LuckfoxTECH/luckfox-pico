#include "SignalingManager.hpp"
#include "udp_signaling.hpp"
#include "base64.hpp"

#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>

SignalingManager::SignalingManager(UdpSignaling& udp,
                                   const std::string& sessionId)
    : udp_(udp), sessionId_(sessionId)
{
}

void SignalingManager::start()
{
    running_ = true;

    udp_.setReceiveCallback(
        [&](const uint8_t* d, size_t l, uint32_t, uint16_t)
        {
            handleUdp(d, l);
        });

    std::thread(&SignalingManager::resendLoop, this).detach();
}

void SignalingManager::stop()
{
    running_ = false;
}

void SignalingManager::onReceive(RxCallback cb)
{
    rxCb_ = cb;
}

void SignalingManager::sendSdp(const std::string& sdp)
{
    sendRaw(SigType::SDP, seq_++, b64_encode(sdp));
}

void SignalingManager::sendIce(const std::string& cand,
                               const std::string& mid)
{
    sendRaw(SigType::ICE, seq_++,
            b64_encode(cand),
            b64_encode(mid));
}

void SignalingManager::sendRaw(SigType type,
                               uint32_t seq,
                               const std::string& p1,
                               const std::string& p2)
{
    std::ostringstream oss;
    oss << (type == SigType::SDP ? "SDP" :
            type == SigType::ICE ? "ICE" : "ACK")
        << "|" << sessionId_
        << "|" << seq
        << "|" << p1;

    if (!p2.empty())
        oss << "|" << p2;

    std::string raw = oss.str();
    udp_.send((uint8_t*)raw.data(), raw.size());

    if (type != SigType::ACK)
        pending_[seq] = {raw, 0};
}

void SignalingManager::handleUdp(const uint8_t* data, size_t len)
{
    std::string msg((char*)data, len);
    std::istringstream iss(msg);

    std::string typeStr, sess;
    uint32_t seq;
    getline(iss, typeStr, '|');
    getline(iss, sess, '|');
    iss >> seq;
    iss.ignore(1);

    if (sess != sessionId_)
        return;

    if (typeStr == "ACK") {
        pending_.erase(seq);
        return;
    }

    // send ACK
    sendRaw(SigType::ACK, seq, "");

    SignalingMsg sm{};
    sm.seq = seq;

    if (typeStr == "SDP") {
        sm.type = SigType::SDP;
        std::getline(iss, sm.payload1);
        sm.payload1 = b64_decode(sm.payload1);
    }
    else if (typeStr == "ICE") {
        sm.type = SigType::ICE;
        std::getline(iss, sm.payload1, '|');
        std::getline(iss, sm.payload2);
        sm.payload1 = b64_decode(sm.payload1);
        sm.payload2 = b64_decode(sm.payload2);
    }

    if (rxCb_)
        rxCb_(sm);
}

void SignalingManager::resendLoop()
{
    while (running_) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(400));

        for (auto& [seq, p] : pending_) {
            if (p.retry++ < 5) {
                udp_.send(
                    (uint8_t*)p.raw.data(),
                    p.raw.size());
            }
        }
    }
}
