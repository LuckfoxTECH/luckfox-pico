#pragma once
#include <rtc/rtc.hpp>
#include <string>
#include <functional>
#include <memory>
#include <vector>

class WebRTCTransport
{
public:
    WebRTCTransport();

    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string,std::string)> cb);
    void onDcOpen(std::function<void()> cb);
    void onDcMessage(std::function<void(const std::string&)> cb);

    void createOffer();
    void createAnswer();
    void setRemoteOffer(const std::string& sdp);
    void setRemoteAnswer(const std::string& sdp);
    void addRemoteIce(const std::string& cand, const std::string& mid);

    void sendMessage(const std::string& msg);

private:
    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel> dc_;

    // ===== state =====
    bool dcOpen_ = false;

    // SDP state
    bool haveRemoteOffer_ = false;   // set sau setRemoteOffer()
    bool answerCreated_   = false;   // set sau createAnswer()

    bool localAnswerSent_ = false;


    // ===== ICE buffer =====
    std::vector<rtc::Candidate> iceBuffer_;

    // ===== callbacks =====
    std::function<void(std::string)> onLocalSdpCb_;
    std::function<void(std::string,std::string)> onLocalIceCb_;
    std::function<void()> onDcOpenCb_;
    std::function<void(const std::string&)> onDcMessageCb_;
};



