#pragma once
#include <rtc/rtc.hpp>
#include <thread>
#include <atomic>
#include "SignalingQueue.hpp"
#include "SignalingEvents.hpp"
#include "udp_signaling.hpp"

namespace signaling {

enum class SigState {
    IDLE,
    OFFER_SENT,
    ICE_EXCHANGING,
    CONNECTED,
    ERROR
};

using SigQueue = SignalingQueue<SigEvent, 32>;

class SignalingManager {
public:
    SignalingManager(rtc::PeerConnection& pc,
                     UdpSignaling& udp);

    ~SignalingManager();

    void start();
    void stop();

    // === events from callbacks ===
    void postLocalSdp(const std::string& sdp);
    void postRemoteSdp(const std::string& sdp);
    void postLocalIce(const std::string& cand,
                      const std::string& mid);
    void postRemoteIce(const std::string& cand,
                       const std::string& mid);
    void postDcOpen();

private:
    void thread_fn();
    void dispatch(const SigEvent& ev);
    void handle_timeout(uint32_t now);

    void send_sdp();
    void send_ice(const std::string&, const std::string&);

    uint32_t now_ms() const;

    rtc::PeerConnection& pc_;
    UdpSignaling& signaling_;

    std::atomic<bool> running_{false};
    std::thread thread_;

    SigState state_{SigState::IDLE};

    SigQueue queue_;

    std::string last_sdp_;
    uint32_t last_tx_ms_{0};
    uint8_t retry_cnt_{0};
};

} // namespace signaling
