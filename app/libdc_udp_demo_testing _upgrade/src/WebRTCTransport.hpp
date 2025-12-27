#pragma once
#include <rtc/rtc.hpp>
#include <functional>
#include <memory>

class WebRTCTransport {
public:
    WebRTCTransport();

    void createOffer();
    void setRemoteAnswer(const std::string& sdp);
    void addRemoteIce(const std::string& cand,
                      const std::string& mid);

    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string,std::string)> cb);
    void onDcOpen(std::function<void()> cb);

private:
    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel> dc_;
};
