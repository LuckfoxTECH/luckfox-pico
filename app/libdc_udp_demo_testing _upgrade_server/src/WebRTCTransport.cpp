#include "WebRTCTransport.hpp"
#include <iostream>

WebRTCTransport::WebRTCTransport()
    : dcOpen_(false),
      haveRemoteOffer_(false),
      answerCreated_(false),
      localAnswerSent_(false)
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    /* ===== LOCAL ICE ===== */
    pc_->onLocalCandidate([this](rtc::Candidate c){
        if (onLocalIceCb_) {
            onLocalIceCb_(c.candidate(), c.mid());
        }
    });

    /* ===== DATA CHANNEL ===== */
    pc_->onDataChannel([this](std::shared_ptr<rtc::DataChannel> dc){
        dc_ = dc;

        dc_->onOpen([this]{
            dcOpen_ = true;
            if (onDcOpenCb_) onDcOpenCb_();
        });

        dc_->onMessage([this](rtc::message_variant msg){
            if (const auto* s = std::get_if<std::string>(&msg)) {
                if (onDcMessageCb_) onDcMessageCb_(*s);
            }
        });
    });
}

/* ===== CALLBACK REG ===== */

void WebRTCTransport::onLocalSdp(
    std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = cb;

    pc_->onLocalDescription([this](rtc::Description d){
        if (!onLocalSdpCb_) return;

        if (d.type() != rtc::Description::Type::Answer)
            return;

        if (localAnswerSent_) {
            std::cout << "[Transport] Ignore duplicate local ANSWER\n";
            return;
        }

        localAnswerSent_ = true;
        onLocalSdpCb_(std::string(d));
    });
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string,std::string)> cb)
{
    onLocalIceCb_ = cb;
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    onDcOpenCb_ = cb;
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    onDcMessageCb_ = cb;
}

/* ===== SDP ===== */

void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    if (haveRemoteOffer_) {
        std::cout << "[Transport] Duplicate OFFER ignored\n";
        return;
    }

    pc_->setRemoteDescription(
        rtc::Description(sdp,
            rtc::Description::Type::Offer));

    haveRemoteOffer_ = true;

    /* ===== FLUSH ICE BUFFER ===== */
    if (!iceBuffer_.empty()) {
        std::cout << "[Transport] Flush ICE buffer: "
                << iceBuffer_.size()
                << " candidate(s)\n";

        for (auto& c : iceBuffer_) {
            std::cout << "[Transport]   + "
                    << c.candidate()
                    << "\n";
            pc_->addRemoteCandidate(c);
        }

        iceBuffer_.clear();
    } else {
        std::cout << "[Transport] ICE buffer empty\n";
    }

}

void WebRTCTransport::createAnswer()
{
    auto state = pc_->signalingState();

    if (state != rtc::PeerConnection::SignalingState::HaveRemoteOffer) {
        std::cout << "[Transport] Cannot create ANSWER, state = "
                  << static_cast<int>(state) << "\n";
        return;
    }

    std::cout << "[Transport] Create ANSWER\n";
    pc_->setLocalDescription(rtc::Description::Type::Answer);
}



void WebRTCTransport::setRemoteAnswer(const std::string& sdp)
{
    pc_->setRemoteDescription(
        rtc::Description(sdp,
            rtc::Description::Type::Answer));
}

/* ===== ICE ===== */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);

    if (haveRemoteOffer_) {
        pc_->addRemoteCandidate(c);
    } else {
        iceBuffer_.push_back(c);
    }
}

/* ===== DATA ===== */

void WebRTCTransport::sendMessage(const std::string& msg)
{
    if (dcOpen_ && dc_) {
        dc_->send(msg);
    }
}
