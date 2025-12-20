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

#include "com/FrameCodec.hpp"

// ============================================================
// TRANSPORT
// ============================================================
#include "transport/ITransport.hpp"
#include "transport/WebRTCTransport.hpp"
// #include "transport/LoopbackTransport.hpp"

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

using u8 = uint8_t;

static void print_frame(const std::vector<u8>& frame)
{
    printf("[FRAME] len=%zu : ", frame.size());

    for (u8 b : frame)
    {
        printf("%02X ", b);
    }

    printf("\n");
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
    // std::signal(SIGINT,  signal_handler);
    // std::signal(SIGTERM, signal_handler);

    // --------------------------------------------------------
    // 2️⃣ RTE / EVENT SYSTEM INIT
    // --------------------------------------------------------
    EventRing<Event,128>::init(g_eventRing);

    // --------------------------------------------------------
    // 3️⃣ APPLICATION SWC INIT
    // --------------------------------------------------------
    ControlState::init();
    SafetyManager::init();

    // --------------------------------------------------------
    // 4️⃣ TRANSPORT INIT
    // --------------------------------------------------------
    //std::unique_ptr<ITransport> transport;

// #if 1
//     // ===== REAL WebRTC =====
//     transport = std::make_unique<WebRTCTransport>();
// #else
//     // ===== LOOPBACK TEST =====
//     transport = std::make_unique<LoopbackTransport>();
// #endif

    // // EventHandler consumes RX frames from transport
    // transport->set_rx_callback([](const std::vector<u8>& data) {
    //     EventHandler::on_transport_rx(data);
    // });

    // transport->start();

    // --------------------------------------------------------
    // 5️⃣ RUNNABLE THREADS (AUTOSAR style)
    // --------------------------------------------------------

    JoystickInput::init();

    JoystickInput joystick;
    joystick.start();

    std::thread t_watchdog([] {
        std::cout << "[RUN] Watchdog\n";
        watchdog::run(g_running);
    });

    std::thread t_event_handler([] {
        std::cout << "[RUN] EventHandler\n";
        EventHandler::run(g_running);
    });

    std::thread t_com_tx([] {
        std::cout << "[RUN] Watchdog\n";
        while (g_running.load())
        {
            ControlSnapshot snap = ControlState::snapshot();

            auto frame =
                FrameCodec::build_fullstate_frame(snap);
            print_frame(frame);
            // transport->send(frame);

            std::this_thread::sleep_for(
                std::chrono::milliseconds(20)
            );
        }
    });

    // --------------------------------------------------------
    // 6️⃣ MAIN IDLE LOOP
    // --------------------------------------------------------
    while (g_running.load())
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(200)
        );
    }

    // --------------------------------------------------------
    // 7️⃣ SHUTDOWN
    // --------------------------------------------------------
    std::cout << "[MAIN] Shutting down...\n";

    //transport->stop();
    joystick.stop();

    // t_joystick.join();
    t_watchdog.join();
    t_event_handler.join();
    t_com_tx.join();

    std::cout << "=====================================\n";
    std::cout << " ECU STOPPED CLEANLY\n";
    std::cout << "=====================================\n";
    return 0;
}
