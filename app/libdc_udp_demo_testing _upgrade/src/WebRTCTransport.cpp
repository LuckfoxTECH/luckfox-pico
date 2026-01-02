#include "WebRTCTransport.hpp"
#include <iostream>

using namespace std::chrono;

WebRTCTransport::WebRTCTransport()
{
    createPeerConnection();
}

/* ================= PEER CONNECTION ================= */

void WebRTCTransport::createPeerConnection()
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    /* ===== Local SDP ===== */
    pc_->onLocalDescription([this](rtc::Description d){
        std::string sdp = std::string(d);
        if (onLocalSdpCb_)
            onLocalSdpCb_(sdp);
        else
            cachedSdp_ = sdp;
    });

    /* ===== Local ICE ===== */
    pc_->onLocalCandidate([this](rtc::Candidate c){
        if (onLocalIceCb_)
            onLocalIceCb_(c.candidate(), c.mid());
    });

    /* ===== Create DC (client side) ===== */
    dc_ = pc_->createDataChannel("dc");

    dc_->onOpen([this]{
        dcOpen_ = true;
        lastPing_ = lastPong_ = steady_clock::now();
        if (onDcOpenCb_) onDcOpenCb_();
    });

    dc_->onClosed([this]{
        dcOpen_ = false;
        if (onDcClosedCb_) onDcClosedCb_();
    });

    dc_->onMessage([this](rtc::message_variant msg){
        if (auto* s = std::get_if<std::string>(&msg)) {
            if (*s == "__ping__") {
                dc_->send("__pong__");
            } else if (*s == "__pong__") {
                lastPong_ = steady_clock::now();
            } else if (onDcMessageCb_) {
                onDcMessageCb_(*s);
            }
        }
    });

    pc_->onIceStateChange([this](rtc::PeerConnection::IceState s){
        std::cout << "[ICE] state=" << static_cast<int>(s) << "\n";
        if (s == rtc::PeerConnection::IceState::Failed ||
            s == rtc::PeerConnection::IceState::Disconnected)
        {
            std::cout << "[ICE] FAILED → full reset\n";
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
    haveRemoteAnswer_ = false;
    iceBuffer_.clear();
    cachedSdp_.reset();
}

/* ================= CALLBACK REG ================= */

void WebRTCTransport::onLocalSdp(std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = std::move(cb);
    if (cachedSdp_) {
        onLocalSdpCb_(*cachedSdp_);
        cachedSdp_.reset();
    }
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string,std::string)> cb)
{
    onLocalIceCb_ = std::move(cb);
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    onDcOpenCb_ = std::move(cb);
}

void WebRTCTransport::onDcClosed(std::function<void()> cb)
{
    onDcClosedCb_ = std::move(cb);
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    onDcMessageCb_ = std::move(cb);
}

/* ================= SIGNALING ================= */

void WebRTCTransport::createOffer()
{
    if (!pc_) return;
    pc_->setLocalDescription(rtc::Description::Type::Offer);
}

void WebRTCTransport::setRemoteAnswer(const std::string& sdp)
{
    pc_->setRemoteDescription(
        rtc::Description(sdp, rtc::Description::Type::Answer));

    haveRemoteAnswer_ = true;

    /* Flush ICE buffer */
    for (auto& c : iceBuffer_)
        pc_->addRemoteCandidate(c);
    iceBuffer_.clear();
}

/* ================= ICE ================= */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);

    if (!pc_ || !haveRemoteAnswer_) {
        iceBuffer_.push_back(c);
        return;
    }

    pc_->addRemoteCandidate(c);
}

/* ================= KEEPALIVE ================= */

void WebRTCTransport::tick()
{
    if (!dcOpen_) return;

    auto now = steady_clock::now();

    if (now - lastPing_ > seconds(3)) {
        dc_->send("__ping__");
        lastPing_ = now;
    }

    if (now - lastPong_ >= seconds(8)) {
        std::cout << "[CONN] timeout → full reset\n";
        close();
    }
}

/* ================= DATA ================= */

void WebRTCTransport::sendMessage(const std::string& msg)
{
    if (dc_ && dc_->isOpen())
        dc_->send(msg);
}

/* ================= FULL RESET ================= */

void WebRTCTransport::close()
{
    destroyPeerConnection();
    createPeerConnection();
    std::cout << "[Transport] session reset\n";
}
