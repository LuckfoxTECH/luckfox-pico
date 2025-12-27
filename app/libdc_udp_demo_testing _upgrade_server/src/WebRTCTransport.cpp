#include "WebRTCTransport.hpp"

WebRTCTransport::WebRTCTransport()
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back(
        "stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);
    dc_ = pc_->createDataChannel("dc");
}

void WebRTCTransport::onLocalSdp(
    std::function<void(std::string)> cb)
{
    pc_->onLocalDescription(
        [cb](rtc::Description d) {
            cb(std::string(d));
        });
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string,std::string)> cb)
{
    pc_->onLocalCandidate(
        [cb](rtc::Candidate c) {
            cb(c.candidate(), c.mid());
        });
}

void WebRTCTransport::onDcOpen(
    std::function<void()> cb)
{
    dc_->onOpen(cb);
}

void WebRTCTransport::createOffer()
{
    pc_->setLocalDescription();
}

void WebRTCTransport::setRemoteAnswer(
    const std::string& sdp)
{
    pc_->setRemoteDescription(
        rtc::Description(
            sdp,
            rtc::Description::Type::Answer));
}

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    pc_->addRemoteCandidate(
        rtc::Candidate(cand, mid));
}

void WebRTCTransport::setRemoteOffer(
    const std::string& sdp)
{
    pc_->setRemoteDescription(
        rtc::Description(
            sdp,
            rtc::Description::Type::Offer));
}

void WebRTCTransport::createAnswer()
{
    pc_->setLocalDescription();
}
