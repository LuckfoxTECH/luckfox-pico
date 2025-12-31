#include "WebRTCTransport.hpp"
#include <iostream>

using namespace std::chrono;

WebRTCTransport::WebRTCTransport()
    : dcOpen_(false),
      haveRemoteOffer_(false)
{
    createPeerConnection();
}

/* ================================
 *  PEER CONNECTION
 * ================================ */

void WebRTCTransport::createPeerConnection()
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    /* ===== LOCAL SDP ===== */
    pc_->onLocalDescription([this](rtc::Description d) {
        if (d.type() != rtc::Description::Type::Answer)
            return;

        std::string sdp = std::string(d);

        if (onLocalSdpCb_)
            onLocalSdpCb_(sdp);
        else
            cachedAnswer_ = sdp;
    });

    /* ===== LOCAL ICE ===== */
    pc_->onLocalCandidate([this](rtc::Candidate c) {
        if (onLocalIceCb_)
            onLocalIceCb_(c.candidate(), c.mid());
    });

    /* ===== DATA CHANNEL ===== */
    pc_->onDataChannel([this](std::shared_ptr<rtc::DataChannel> dc) {
        dc_ = dc;

        dc_->onOpen([this] {
            dcOpen_ = true;
            lastPing_ = lastPong_ = steady_clock::now();
            if (onDcOpenCb_) onDcOpenCb_();
        });

        dc_->onClosed([this] {
            std::cout << "[DC] closed → reset session\n";
            close();
        });

        dc_->onMessage([this](rtc::message_variant msg) {
            if (auto* s = std::get_if<std::string>(&msg)) {

                if (*s == "__ping__") {
                    dc_->send("__pong__");
                    return;
                }
                if (*s == "__pong__") {
                    lastPong_ = steady_clock::now();
                    return;
                }
                if (onDcMessageCb_)
                    onDcMessageCb_(*s);
            }
        });
        
    });

    /* ===== ICE STATE ===== */
    pc_->onIceStateChange([this](rtc::PeerConnection::IceState s) {
        std::cout << "[ICE] state=" << static_cast<int>(s) << "\n";

        if (s == rtc::PeerConnection::IceState::Failed) {
            std::cout << "[ICE] FAILED → reset session\n";
            close();
        }
    });
}

void WebRTCTransport::destroyPeerConnection()
{
    try {
        if (dc_) dc_->close();
        if (pc_) pc_->close();
    } catch (...) {}

    pc_.reset();
    dc_.reset();

    dcOpen_ = false;
    haveRemoteOffer_ = false;
    iceBuffer_.clear();
    cachedAnswer_.reset();
}

/* ================================
 *  CALLBACK REG
 * ================================ */

void WebRTCTransport::onLocalSdp(std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = std::move(cb);
    if (cachedAnswer_) {
        onLocalSdpCb_(*cachedAnswer_);
        cachedAnswer_.reset();
    }
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string, std::string)> cb)
{
    onLocalIceCb_ = std::move(cb);
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    onDcOpenCb_ = std::move(cb);
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    onDcMessageCb_ = std::move(cb);
}

/* ================================
 *  SIGNALING
 * ================================ */

void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    std::cout << "[SDP] New OFFER → reset session\n";

    destroyPeerConnection();
    createPeerConnection();

    pc_->setRemoteDescription(
        rtc::Description(sdp, rtc::Description::Type::Offer));

    haveRemoteOffer_ = true;

    for (auto& c : iceBuffer_)
        pc_->addRemoteCandidate(c);
    iceBuffer_.clear();

    createAnswer();
}

void WebRTCTransport::createAnswer()
{
    if (!pc_) return;

    if (pc_->signalingState() !=
        rtc::PeerConnection::SignalingState::HaveRemoteOffer) {
        return;
    }

    std::cout << "[SDP] Create ANSWER\n";
    pc_->setLocalDescription(rtc::Description::Type::Answer);
}

/* ================================
 *  ICE
 * ================================ */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);

    if (!pc_ || !haveRemoteOffer_) {
        iceBuffer_.push_back(c);
        return;
    }

    try {
        pc_->addRemoteCandidate(c);
    } catch (...) {}
}

/* ================================
 *  KEEPALIVE
 * ================================ */

void WebRTCTransport::tick()
{
    if (!dcOpen_) return;

    auto now = steady_clock::now();

    if (now - lastPing_ > seconds(3)) {
        dc_->send("__ping__");
        lastPing_ = now;
    }

    if (now - lastPong_ > seconds(10)) {
        std::cout << "[CONN] timeout → reset session\n";
        close();
    }
}

/* ================================
 *  DATA
 * ================================ */

void WebRTCTransport::sendMessage(const std::string& msg)
{
    if (!dc_ || !dc_->isOpen())
        return;
    dc_->send(msg);
}

/* ================================
 *  CLOSE
 * ================================ */

void WebRTCTransport::close()
{
    destroyPeerConnection();
    createPeerConnection();
    std::cout << "[Transport] Session reset\n";
}
