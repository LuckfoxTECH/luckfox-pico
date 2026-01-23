#pragma once

#include <rtc/rtc.hpp>

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <optional>
#include <chrono>
#include <atomic>

// ============================================================
// WebRTCTransport
//  - WebRTC DataChannel transport
//  - Support string + binary (frame)
//  - Keepalive + auto reset
// ============================================================

class WebRTCTransport
{
public:
    WebRTCTransport();
    ~WebRTCTransport() = default;

    /* ========================================================
     * Callback registration
     * ======================================================== */

    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string, std::string)> cb);

    void onDcOpen(std::function<void()> cb);
    void onDcClosed(std::function<void()> cb);

    // text message (control / debug)
    void onDcMessage(std::function<void(const std::string&)> cb);

    // binary message (frame)
    void onDcBinary(
        std::function<void(const std::vector<uint8_t>&)> cb);

    /* ========================================================
     * Signaling
     * ======================================================== */

    void createOffer();
    void setRemoteAnswer(const std::string& sdp);
    bool haveLocalOffer() const;

    /* ========================================================
     * ICE
     * ======================================================== */

    void addRemoteIce(const std::string& cand,
                      const std::string& mid);

    /* ========================================================
     * Data TX
     * ======================================================== */

    // text (legacy / debug)
    void sendMessage(const std::string& msg);

    // binary frame (CONTROL / SENSOR / STATE)
    void sendBinary(const std::vector<uint8_t>& data);

    /* ========================================================
     * State / Lifecycle
     * ======================================================== */

    bool isReady() const;        // DC open & usable
    void tick();                 // keepalive + timeout
    void close();                // full reset (PC + DC)

private:
    /* ========================================================
     * Internal helpers
     * ======================================================== */

    void createPeerConnection();
    void destroyPeerConnection();

private:
    /* ========================================================
     * Core WebRTC
     * ======================================================== */

    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel>    dc_;

    /* ========================================================
     * State
     * ======================================================== */

    std::atomic<bool> dcOpen_{false};
    bool haveRemoteAnswer_{false};

    /* ========================================================
     * ICE
     * ======================================================== */

    std::vector<rtc::Candidate> iceBuffer_;

    /* ========================================================
     * SDP
     * ======================================================== */

    std::optional<std::string> cachedSdp_;

    /* ========================================================
     * Keepalive
     * ======================================================== */

    std::chrono::steady_clock::time_point lastPing_;
    std::chrono::steady_clock::time_point lastPong_;

    /* ========================================================
     * Callbacks
     * ======================================================== */

    // signaling
    std::function<void(std::string)> onLocalSdpCb_;
    std::function<void(std::string, std::string)> onLocalIceCb_;

    // data channel lifecycle
    std::function<void()> onDcOpenCb_;
    std::function<void()> onDcClosedCb_;

    // data receive
    std::function<void(const std::string&)> onDcMessageCb_;
    std::function<void(const std::vector<uint8_t>&)>
        onDcBinaryCb_;
};
