#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>

#include "app/ControlState.hpp"
#include "app/IoctlController.hpp"

#include "signaling/udp_signaling.hpp"
#include "signaling/SignalingManager.hpp"
#include "signaling/PeerServer.hpp"
#include "transport/WebRTCTransport.hpp"

std::atomic<bool> app_running{true};

int main()
{
    using namespace std::chrono_literals;

    std::cout << "[MAIN] Starting...\n";

    ControlState::init();

    IoctlController ioctlCtrl;
    ioctlCtrl.start();

    UdpSignaling udp;
    udp.init(nullptr, 6000, nullptr, 0);
    udp.start();

    SignalingManager sig(udp, "sess1");
    WebRTCTransport rtc;
    PeerServer server(sig, rtc);
    server.start();

    while (app_running.load())
    {
        std::this_thread::sleep_for(1s);
    }

    std::cout << "[MAIN] Shutting down...\n";
    ioctlCtrl.stop();

    return 0;
}

