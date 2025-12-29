#include "WebRTCTransport.hpp"
#include <iostream>  

WebRTCTransport::WebRTCTransport()
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);
    dc_ = pc_->createDataChannel("dc");

    dc_->onMessage([this](rtc::message_variant msg){
        if(const auto* s = std::get_if<std::string>(&msg)){
            std::cout << "[WebRTC] DC message received: " << *s << "\n";
        }
    });
}

void WebRTCTransport::onLocalSdp(std::function<void(std::string)> cb)
{
    pc_->onLocalDescription([cb](rtc::Description d){
        cb(std::string(d));
    });
}

void WebRTCTransport::onLocalIce(std::function<void(std::string,std::string)> cb)
{
    pc_->onLocalCandidate([cb](rtc::Candidate c){
        cb(c.candidate(), c.mid());
    });
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    dc_->onOpen([this, cb]{
        dcOpen_ = true;   // update trạng thái
        cb();
    });
}

void WebRTCTransport::onDcMessage(std::function<void(const std::string&)> cb)
{
    dc_->onMessage([cb](rtc::message_variant msg){
        if(const auto* s = std::get_if<std::string>(&msg)){
            cb(*s);
        }
    });
}

void WebRTCTransport::createOffer()
{
    pc_->setLocalDescription();
}

void WebRTCTransport::createAnswer()
{
    // tạo Answer dựa trên remote Offer
    pc_->setLocalDescription(rtc::Description::Type::Answer);
}


void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    pc_->setRemoteDescription(rtc::Description(sdp, rtc::Description::Type::Offer));
}

void WebRTCTransport::setRemoteAnswer(const std::string& sdp)
{
    pc_->setRemoteDescription(rtc::Description(sdp, rtc::Description::Type::Answer));
}

void WebRTCTransport::addRemoteIce(const std::string& cand, const std::string& mid)
{
    pc_->addRemoteCandidate(rtc::Candidate(cand, mid));
}

void WebRTCTransport::sendMessage(const std::string& msg)
{
    if(dcOpen_){        
        dc_->send(msg);
        std::cout << "[WebRTC] DC message sent: " << msg << "\n";
    }
}

