#include "WebRTCTransport.hpp"
#include <iostream>

using namespace std::chrono;

WebRTCTransport::WebRTCTransport() {
    createPeerConnection();
}

/* ================= PEER CONNECTION ================= */

void WebRTCTransport::createPeerConnection()
{
    rtc::Configuration cfg;

    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    pc_->onLocalDescription([this](rtc::Description d){
        if (d.type() != rtc::Description::Type::Answer)
            return;

        std::string sdp = std::string(d);
        if (onLocalSdpCb_)
            onLocalSdpCb_(sdp);
        else
            cachedAnswer_ = sdp;
    });

    pc_->onLocalCandidate([this](rtc::Candidate c){
        if (onLocalIceCb_)
            onLocalIceCb_(c.candidate(), c.mid());
    });

    pc_->onDataChannel([this](std::shared_ptr<rtc::DataChannel> dc){
        dc_ = dc;

        dc_->onOpen([this]{
            dcOpen_ = true;
            iceRestarted_ = false;
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
    });

    pc_->onIceStateChange([this](rtc::PeerConnection::IceState s){
        std::cout << "[ICE] state=" << static_cast<int>(s) << "\n";
        if (s == rtc::PeerConnection::IceState::Failed ||
            s == rtc::PeerConnection::IceState::Disconnected) {
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
    haveRemoteOffer_ = false;
    iceRestarted_ = false;
    iceBuffer_.clear();
    cachedAnswer_.reset();
}

/* ================= CALLBACK REG ================= */

void WebRTCTransport::onLocalSdp(std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = std::move(cb);
    if (cachedAnswer_) {
        onLocalSdpCb_(*cachedAnswer_);
        cachedAnswer_.reset();
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

void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    std::cout << "[SDP] NEW OFFER → full reset\n";

    destroyPeerConnection();
    createPeerConnection();

    pc_->setRemoteDescription(
        rtc::Description(sdp, rtc::Description::Type::Offer));

    haveRemoteOffer_ = true;
    createAnswer();
}

void WebRTCTransport::createAnswer()
{
    if (!pc_) return;
    if (pc_->signalingState() ==
        rtc::PeerConnection::SignalingState::HaveRemoteOffer)
    {
        pc_->setLocalDescription(
            rtc::Description::Type::Answer);
    }
}

/* ================= ICE ================= */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);
    if (!pc_ || !haveRemoteOffer_) {
        iceBuffer_.push_back(c);
        return;
    }
    pc_->addRemoteCandidate(c);
}

/* ================= KEEPALIVE ================= */

void WebRTCTransport::tryIceRestart()
{
    if (iceRestarted_ || !pc_) return;
    std::cout << "[ICE] restart\n";
    //pc_->restartIce();
    iceRestarted_ = true;
}

void WebRTCTransport::tick()
{
    if (!dcOpen_) return;

    auto now = steady_clock::now();

    if (now - lastPing_ > seconds(3)) {
        dc_->send("__ping__");
        lastPing_ = now;
    }

    auto lost = now - lastPong_;

    if (lost > seconds(5) && lost < seconds(8)) {
        tryIceRestart();        // network glitch
    }

    if (lost >= seconds(8)) {
        std::cout << "[CONN] timeout → full reset\n";
        close();                // client reset / dead
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
