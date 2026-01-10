#include <thread>
#include <chrono>

#include "app/ControlState.hpp"

#include "signaling/udp_signaling.hpp"
#include "signaling/SignalingManager.hpp"
#include "signaling/PeerServer.hpp"

#include "transport/WebRTCTransport.hpp"

int main()
{

    // --------------------------------------------------------
    // 2 RTE / EVENT SYSTEM INIT
    // --------------------------------------------------------
    // EventRing<Event,128>::init(g_eventRing);

    // --------------------------------------------------------
    // 3 APPLICATION SWC INIT
    // --------------------------------------------------------
    ControlState::init();

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
    

    // --------------------------------------------------------
    // 7️⃣ SHUTDOWN
    // --------------------------------------------------------
    std::cout << "[MAIN] Shutting down...\n";


    // t_transport_tick.join();

}
