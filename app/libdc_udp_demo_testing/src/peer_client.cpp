#include <rtc/rtc.hpp>
#include <iostream>
#include <thread>

#include "udp_signaling.hpp"
#include "base64.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "peer_client <server_ip>\n";
        return 0;
    }

    UdpSignaling signaling;
    signaling.init(nullptr, 0, argv[1], 6000);
    signaling.start();

    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(cfg);
    bool answerOnce = false;

    auto dc = pc->createDataChannel("dc");

    dc->onOpen([]()
    {
        std::cout << "[Client] DC OPEN\n";
    });

    pc->onLocalDescription([&](rtc::Description desc)
    {
        std::string msg =
            "SDP|" + b64_encode(std::string(desc));

        signaling.send(
            (uint8_t*)msg.data(),
            msg.size());

        std::cout << "[Client] Sent OFFER\n";
    });

    pc->onLocalCandidate([&](rtc::Candidate c)
    {
        std::string msg =
            "ICE|" + b64_encode(c.candidate()) +
            "|"  + b64_encode(c.mid());

        signaling.send(
            (uint8_t*)msg.data(),
            msg.size());
    });

    signaling.setReceiveCallback(
        [&](const uint8_t* data,
            size_t len,
            uint32_t,
            uint16_t)
        {
            std::string msg((char*)data, len);

            if (!answerOnce &&
                msg.rfind("SDP|", 0) == 0)
            {
                pc->setRemoteDescription(
                    rtc::Description(
                        b64_decode(msg.substr(4)),
                        rtc::Description::Type::Answer));

                answerOnce = true;
                std::cout << "[Client] Got ANSWER\n";
            }
            else if (msg.rfind("ICE|", 0) == 0)
            {
                auto p = msg.find('|', 4);
                pc->addRemoteCandidate(
                    rtc::Candidate(
                        b64_decode(msg.substr(4, p - 4)),
                        b64_decode(msg.substr(p + 1))));
            }
        });

    pc->setLocalDescription();

    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
