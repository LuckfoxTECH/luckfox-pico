#include "WebRTCTransport.hpp"
#include <iostream>
#include <utility>

/* ================================
 *  Constructor
 * ================================ */
WebRTCTransport::WebRTCTransport()
    : dcOpen_(false),
      haveRemoteOffer_(false),
      localAnswerSent_(false)
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    /* =====================================================
     *  LOCAL SDP – REGISTER ONCE (CRITICAL FIX)
     * ===================================================== */
    pc_->onLocalDescription([this](rtc::Description d) {
        if (d.type() != rtc::Description::Type::Answer)
            return;

        if (localAnswerSent_) {
            std::cout << "[Transport] Duplicate local ANSWER ignored\n";
            return;
        }

        localAnswerSent_ = true;

        std::string sdp = std::string(d);

        if (onLocalSdpCb_) {
            onLocalSdpCb_(sdp);
        } else {
            /* ultra-safe: cache if callback registered late */
            cachedAnswer_ = sdp;
            std::cout << "[Transport] ANSWER cached (no callback yet)\n";
        }
    });

    /* =====================================================
     *  LOCAL ICE
     * ===================================================== */
    pc_->onLocalCandidate([this](rtc::Candidate c) {
        if (onLocalIceCb_) {
            onLocalIceCb_(c.candidate(), c.mid());
        }
    });

    /* =====================================================
     *  DATA CHANNEL
     * ===================================================== */
    pc_->onDataChannel([this](std::shared_ptr<rtc::DataChannel> dc) {
        dc_ = dc;

        dc_->onOpen([this] {
            dcOpen_ = true;
            if (onDcOpenCb_)
                onDcOpenCb_();
        });

        dc_->onClosed([this] {
            dcOpen_ = false;
            std::cout << "[Transport] DataChannel closed\n";
        });

        dc_->onMessage([this](rtc::message_variant msg) {
            if (const auto* s = std::get_if<std::string>(&msg)) {
                if (onDcMessageCb_)
                    onDcMessageCb_(*s);
            }
        });
    });
}

/* ================================
 *  CALLBACK REGISTRATION
 * ================================ */

void WebRTCTransport::onLocalSdp(
    std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = std::move(cb);

    /* flush cached ANSWER if any */
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

void WebRTCTransport::onDcOpen(
    std::function<void()> cb)
{
    onDcOpenCb_ = std::move(cb);
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    onDcMessageCb_ = std::move(cb);
}

/* ================================
 *  SDP
 * ================================ */

void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    if (haveRemoteOffer_) {
        std::cout << "[Transport] Duplicate OFFER ignored\n";
        return;
    }

    std::cout << "[Transport] Set remote OFFER\n";

    pc_->setRemoteDescription(
        rtc::Description(sdp,
            rtc::Description::Type::Offer));

    haveRemoteOffer_ = true;

    /* ===== Flush ICE buffer ===== */
    if (!iceBuffer_.empty()) {
        std::cout << "[Transport] Flush ICE buffer: "
                << iceBuffer_.size()
                << " candidate(s)\n";

        for (auto& c : iceBuffer_) {
            pc_->addRemoteCandidate(c);
        }
        iceBuffer_.clear();
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
    std::cout << "[Transport] Set remote ANSWER\n";

    pc_->setRemoteDescription(
        rtc::Description(sdp,
            rtc::Description::Type::Answer));
}

/* ================================
 *  ICE
 * ================================ */

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

/* ================================
 *  DATA
 * ================================ */

void WebRTCTransport::sendMessage(const std::string& msg)
{
    if (dcOpen_ && dc_) {
        dc_->send(msg);
    }
}
