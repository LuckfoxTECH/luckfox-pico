#include <thread>
#include <chrono>

#include "udp_signaling.hpp"
#include "SignalingManager.hpp"
#include "WebRTCTransport.hpp"
#include "PeerServer.hpp"

int main()
{
    UdpSignaling udp;
    udp.init(nullptr, 6000, nullptr, 0);
    udp.start();

    SignalingManager sig(udp, "sess1");

    WebRTCTransport rtc;
    PeerServer server(sig, rtc);

    server.start();

    while (true)
        std::this_thread::sleep_for(
            std::chrono::seconds(1));
}
