#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include <chrono>
#include <memory>

// ============================================================
// COMMON / RTE
// ============================================================
#include "app/EventRing.hpp"

// ============================================================
// APPLICATION SWC
// ============================================================
#include "app/ControlState.hpp"
#include "app/SafetyManager.hpp"
#include "app/EventHandler.hpp"
#include "app/VehicleState.hpp"


#include "com/FrameCodec.hpp"

// ============================================================
// TRANSPORT
// ============================================================
#include "transport/ITransport.hpp"
#include "transport/WebRTCTransport.hpp"

// signaling
#include "signaling/udp_signaling.hpp"
#include "signaling/SignalingManager.hpp"
#include "PeerClient.hpp"

// ============================================================
// IO
// ============================================================
#include "io/JoystickInput.hpp"
#include "io/Watchdog.hpp"

// ============================================================
// GLOBALS
// ============================================================
static std::atomic<bool> g_running{true};

// ============================================================
// SIGNAL HANDLER
// ============================================================
static void signal_handler(int)
{
    std::cout << "\n[MAIN] Shutdown requested\n";
    g_running.store(false);
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char* argv[])
{
    std::cout << "=====================================\n";
    std::cout << " AUTOSAR-like WebRTC Control ECU START\n";
    std::cout << "=====================================\n";

    // --------------------------------------------------------
    // 1️⃣ POSIX SIGNALS
    // --------------------------------------------------------
    std::signal(SIGINT,  signal_handler);
    std::signal(SIGTERM, signal_handler);

    // --------------------------------------------------------
    // 2️⃣ RTE / EVENT SYSTEM INIT
    // --------------------------------------------------------
    EventRing<Event,128>::init(g_eventRing);

    // --------------------------------------------------------
    // 3️⃣ APPLICATION SWC INIT
    // --------------------------------------------------------
    ControlState::init();
    SafetyManager::init();
    VehicleState::init();

    // --------------------------------------------------------
    // 4️⃣ TRANSPORT + SIGNALING INIT
    // --------------------------------------------------------
    if (argc < 2) {
        std::cout << "Usage: peer_client <peer_ip>\n";
        return 0;
    }

    // --- signaling ---
    UdpSignaling udp;
    udp.init(nullptr, 0, argv[1], 6000);
    udp.start();

    SignalingManager sig(udp, "sess1");

    // --- transport ---
    auto transport = std::make_shared<WebRTCTransport>();

    // --- peer client (offerer) ---
    PeerClient client(sig, *transport);
    client.start();

    // --------------------------------------------------------
    // 5️⃣ RUNNABLE THREADS (AUTOSAR-style)
    // --------------------------------------------------------

    // ----- Joystick -----
    JoystickInput::init();
    JoystickInput joystick;
    joystick.start();

    // ----- Transport tick (keepalive / timeout) -----
    std::thread t_transport_tick([&] {
        while (g_running.load()) {
            transport->tick();
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50));
        }
    });

    // ----- Watchdog -----
    std::thread t_watchdog([] {
        std::cout << "[RUN] Watchdog\n";
        watchdog::run(g_running);
    });

    // ----- Event handler -----
    std::thread t_event_handler([] {
        std::cout << "[RUN] EventHandler\n";
        EventHandler::run(g_running);
    });

    // ----- COM TX (20 ms) -----
    constexpr auto TX_PERIOD = std::chrono::milliseconds(20); // 50 Hz
    constexpr auto MAX_SILENT = std::chrono::milliseconds(200);

    std::thread t_com_tx(
        [transport, TX_PERIOD, MAX_SILENT] {
            ControlSnapshot last{};
            auto last_sent = std::chrono::steady_clock::now();

            while (g_running.load()) {
                auto now = std::chrono::steady_clock::now();
                ControlSnapshot cur = ControlState::snapshot();

                bool changed =
                    cur.steering  != last.steering  ||
                    cur.throttle  != last.throttle  ||
                    cur.brake     != last.brake     ||
                    cur.direction != last.direction;

                bool timeout =
                    (now - last_sent) >= MAX_SILENT;

                if (transport->isReady() && (changed || timeout)) {
                    auto frame =
                        FrameCodec::build_fullstate_frame(cur);
                    if (!frame.empty())
                        transport->sendBinary(frame);

                    last = cur;
                    last_sent = now;
                }

                std::this_thread::sleep_for(TX_PERIOD);
            }
        }
    );



    // --------------------------------------------------------
    // 6️⃣ MAIN IDLE LOOP
    // --------------------------------------------------------
    while (g_running.load()) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(200));
    }

    // --------------------------------------------------------
    // 7️⃣ SHUTDOWN
    // --------------------------------------------------------
    std::cout << "[MAIN] Shutting down...\n";

    joystick.stop();

    t_transport_tick.join();
    t_watchdog.join();
    t_event_handler.join();
    t_com_tx.join();

    std::cout << "=====================================\n";
    std::cout << " ECU STOPPED CLEANLY\n";
    std::cout << "=====================================\n";

    return 0;
}
