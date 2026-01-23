#pragma once

#include <rtc/rtc.hpp>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <optional>
#include <chrono>

class WebRTCTransport
{
public:
    WebRTCTransport();

    /* ===== Callback registration ===== */
    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string, std::string)> cb);

    void onDcOpen(std::function<void()> cb);
    void onDcClosed(std::function<void()> cb);
    void onDcMessage(std::function<void(const std::string&)> cb);

    /* ===== Signaling ===== */
    void createOffer();                     
    void setRemoteAnswer(const std::string& sdp);

    /* ===== ICE ===== */
    void addRemoteIce(const std::string& cand,
                      const std::string& mid);

    /* ===== Data ===== */
    void sendMessage(const std::string& msg);

    /* ===== Lifecycle ===== */
    void tick();                            // keepalive
    void close();                           // full reset

private:
    /* ===== Internal helpers ===== */
    void createPeerConnection();
    void destroyPeerConnection();

private:
    /* ===== Core ===== */
    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel>    dc_;

    /* ===== State ===== */
    bool dcOpen_            = false;
    bool haveRemoteAnswer_  = false;

    /* ===== ICE ===== */
    std::vector<rtc::Candidate> iceBuffer_;

    /* ===== SDP ===== */
    std::optional<std::string> cachedSdp_;

    /* ===== Keepalive ===== */
    std::chrono::steady_clock::time_point lastPing_;
    std::chrono::steady_clock::time_point lastPong_;

    /* ===== Callbacks ===== */
    std::function<void(std::string)> onLocalSdpCb_;
    std::function<void(std::string, std::string)> onLocalIceCb_;
    std::function<void()> onDcOpenCb_;
    std::function<void()> onDcClosedCb_;
    std::function<void(const std::string&)> onDcMessageCb_;
};
