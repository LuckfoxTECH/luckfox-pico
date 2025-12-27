#include <rtc/rtc.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#include "udp_signaling.hpp"
#include "base64.hpp"

int main()
{
    std::cout << "[Server] Ready on UDP 6000\n";

    /* ---------- UDP signaling ---------- */
    UdpSignaling signaling;
    signaling.init(nullptr, 6000, nullptr, 0);
    signaling.start();

    /* ---------- PeerConnection ---------- */
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(cfg);

    /* ---------- State ---------- */
    bool remoteSet   = false;
    bool answerSent  = false;
    std::vector<rtc::Candidate> iceBuf;

    /* ---------- DataChannel ---------- */
    pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> dc)
    {
        std::cout << "[Server] DC received\n";

        dc->onOpen([]()
        {
            std::cout << "[Server] DC OPEN\n";
        });

        dc->onMessage([](rtc::message_variant msg)
        {
            if (const auto* s = std::get_if<std::string>(&msg))
                std::cout << "[Server] DC msg: " << *s << "\n";
        });
    });

    /* ---------- Local SDP ---------- */
    pc->onLocalDescription([&](rtc::Description desc)
    {
        /* CHỈ GỬI ANSWER */
        if (desc.type() != rtc::Description::Type::Answer)
            return;

        if (answerSent)
        {
            std::cout << "[Server] Ignore duplicate ANSWER\n";
            return;
        }

        answerSent = true;

        std::string msg =
            "SDP|" + b64_encode(std::string(desc));

        signaling.send(
            reinterpret_cast<const uint8_t*>(msg.data()),
            msg.size());

        std::cout << "[Server] Sent ANSWER\n";
    });

    /* ---------- Local ICE ---------- */
    pc->onLocalCandidate([&](rtc::Candidate c)
    {
        std::string msg =
            "ICE|" + b64_encode(c.candidate()) +
            "|"   + b64_encode(c.mid());

        signaling.send(
            reinterpret_cast<const uint8_t*>(msg.data()),
            msg.size());
    });

    /* ---------- Receive signaling ---------- */
    signaling.setReceiveCallback(
        [&](const uint8_t* data,
            size_t len,
            uint32_t,
            uint16_t)
        {
            std::string msg((const char*)data, len);

            /* ===== SDP ===== */
            if (msg.rfind("SDP|", 0) == 0)
            {
                if (remoteSet)
                {
                    std::cout << "[Server] Ignore duplicate OFFER\n";
                    return;
                }

                pc->setRemoteDescription(
                    rtc::Description(
                        b64_decode(msg.substr(4)),
                        rtc::Description::Type::Offer));

                remoteSet = true;
                std::cout << "[Server] Got OFFER\n";

                /* Flush buffered ICE */
                for (auto& c : iceBuf)
                    pc->addRemoteCandidate(c);
                iceBuf.clear();

                /* Create ANSWER */
                pc->setLocalDescription();
            }
            /* ===== ICE ===== */
            else if (msg.rfind("ICE|", 0) == 0)
            {
                auto p = msg.find('|', 4);

                rtc::Candidate c(
                    b64_decode(msg.substr(4, p - 4)),
                    b64_decode(msg.substr(p + 1)));

                if (!remoteSet)
                    iceBuf.push_back(c);
                else
                    pc->addRemoteCandidate(c);
            }
        });

    /* ---------- Loop ---------- */
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
