#include "udp_signaling.hpp"
#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"
#include "PeerClient.hpp"

#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "peer_client <peer_ip>\n";
        return 0;
    }

    UdpSignaling udp;
    udp.init(nullptr, 0, argv[1], 6000);
    udp.start();

    SignalingManager sig(udp, "sess1");
    WebRTCTransport rtc;
    PeerClient client(sig, rtc);

    client.start();

    while (true)
        std::this_thread::sleep_for(
            std::chrono::seconds(1));
}
