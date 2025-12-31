#pragma once
#include <rtc/rtc.hpp>
#include <functional>
#include <optional>
#include <vector>
#include <chrono>

class WebRTCTransport {
public:
    WebRTCTransport();

    /* callbacks */
    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string, std::string)> cb);
    void onDcOpen(std::function<void()> cb);
    void onDcMessage(std::function<void(const std::string&)> cb);
    

    /* signaling */
    void setRemoteOffer(const std::string& sdp);
    void addRemoteIce(const std::string& cand, const std::string& mid);

    /* runtime */
    void tick();
    void sendMessage(const std::string& msg);
    void close();

private:
    void createPeerConnection();
    void destroyPeerConnection();
    void createAnswer();

private:
    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel> dc_;

    std::vector<rtc::Candidate> iceBuffer_;

    bool dcOpen_;
    bool haveRemoteOffer_;

    std::optional<std::string> cachedAnswer_;

    std::function<void(std::string)> onLocalSdpCb_;
    std::function<void(std::string, std::string)> onLocalIceCb_;
    std::function<void()> onDcOpenCb_;
    std::function<void(const std::string&)> onDcMessageCb_;

    std::chrono::steady_clock::time_point lastPing_;
    std::chrono::steady_clock::time_point lastPong_;
};
