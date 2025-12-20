#include "SignalingManager.hpp"
#include <chrono>
#include <iostream>

namespace signaling {

static constexpr uint32_t SIG_PERIOD_MS  = 10;
static constexpr uint32_t SIG_TIMEOUT_MS = 2000;
static constexpr uint8_t  SIG_MAX_RETRY  = 3;

// ============================================================

SignalingManager::SignalingManager(rtc::PeerConnection& pc,
                                   UdpSignaling& udp)
    : pc_(pc)
    , signaling_(udp)
{
}

SignalingManager::~SignalingManager()
{
    stop();
}

void SignalingManager::start()
{
    running_ = true;
    thread_ = std::thread(&SignalingManager::thread_fn, this);
}

void SignalingManager::stop()
{
    running_ = false;
    if (thread_.joinable())
        thread_.join();
}

// ============================================================
// POST EVENTS
// ============================================================

void SignalingManager::postLocalSdp(const std::string& sdp)
{
    queue_.push({SigEventType::LOCAL_SDP, sdp, {}, now_ms()});
}

void SignalingManager::postRemoteSdp(const std::string& sdp)
{
    queue_.push({SigEventType::REMOTE_SDP, sdp, {}, now_ms()});
}

void SignalingManager::postLocalIce(const std::string& c,
                                    const std::string& m)
{
    queue_.push({SigEventType::LOCAL_ICE, c, m, now_ms()});
}

void SignalingManager::postRemoteIce(const std::string& c,
                                     const std::string& m)
{
    queue_.push({SigEventType::REMOTE_ICE, c, m, now_ms()});
}

void SignalingManager::postDcOpen()
{
    queue_.push({SigEventType::DC_OPEN, {}, {}, now_ms()});
}

// ============================================================

void SignalingManager::thread_fn()
{
    while (running_) {
        SigEvent ev;
        while (queue_.pop(ev))
            dispatch(ev);

        handle_timeout(now_ms());
        std::this_thread::sleep_for(
            std::chrono::milliseconds(SIG_PERIOD_MS));
    }
}

// ============================================================
// FSM
// ============================================================

void SignalingManager::dispatch(const SigEvent& ev)
{
    switch (state_) {

    case SigState::IDLE:
        if (ev.type == SigEventType::LOCAL_SDP) {
            last_sdp_ = ev.p1;
            send_sdp();
            retry_cnt_ = 0;
            last_tx_ms_ = ev.ts;
            state_ = SigState::OFFER_SENT;
        }
        break;

    case SigState::OFFER_SENT:
        if (ev.type == SigEventType::REMOTE_SDP) {
            pc_.setRemoteDescription(
                rtc::Description(ev.p1));
            state_ = SigState::ICE_EXCHANGING;
        }
        break;

    case SigState::ICE_EXCHANGING:
        if (ev.type == SigEventType::LOCAL_ICE)
            send_ice(ev.p1, ev.p2);
        else if (ev.type == SigEventType::REMOTE_ICE)
            pc_.addRemoteCandidate(
                rtc::Candidate(ev.p1, ev.p2));
        else if (ev.type == SigEventType::DC_OPEN)
            state_ = SigState::CONNECTED;
        break;

    default:
        break;
    }
}

// ============================================================

void SignalingManager::handle_timeout(uint32_t now)
{
    if (state_ == SigState::OFFER_SENT &&
        now - last_tx_ms_ > SIG_TIMEOUT_MS)
    {
        if (++retry_cnt_ <= SIG_MAX_RETRY) {
            send_sdp();
            last_tx_ms_ = now;
        } else {
            state_ = SigState::ERROR;
            std::cerr << "[SIG] SDP timeout\n";
        }
    }
}

// ============================================================
// TX (NO BASE64 HERE)
// ============================================================

void SignalingManager::send_sdp()
{
    signaling_.sendSdp(last_sdp_);
}

void SignalingManager::send_ice(const std::string& c,
                                const std::string& m)
{
    signaling_.sendIce(c, m);
}

// ============================================================

uint32_t SignalingManager::now_ms() const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count();
}

} // namespace signaling
