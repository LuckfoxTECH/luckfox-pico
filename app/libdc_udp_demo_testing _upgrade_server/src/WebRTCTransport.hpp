#pragma once

#include <rtc/rtc.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <optional>
#include <string>

class WebRTCTransport {
public:
    WebRTCTransport();

    void onLocalSdp(std::function<void(std::string)> cb);
    void onLocalIce(std::function<void(std::string,std::string)> cb);
    void onDcOpen(std::function<void()> cb);
    void onDcMessage(std::function<void(const std::string&)> cb);

    void setRemoteOffer(const std::string& sdp);
    void createAnswer();
    void setRemoteAnswer(const std::string& sdp);

    void addRemoteIce(const std::string& cand,
                      const std::string& mid);

    void sendMessage(const std::string& msg);

    void close();

private:

    /* ================================
     *  Signaling State (STRONG)
     * ================================ */
    enum class TransportState {
        Idle,               // initial
        HaveRemoteOffer,    // setRemoteOffer done
        AnswerSent,         // onLocalDescription(ANSWER)
        Connected,          // ICE + DTLS up (optional)
        Closed
    };

    TransportState state_ = TransportState::Idle;
    
    /* WebRTC core */
    std::shared_ptr<rtc::PeerConnection> pc_;
    std::shared_ptr<rtc::DataChannel> dc_;

    /* Callbacks */
    std::function<void(std::string)> onLocalSdpCb_;
    std::function<void(std::string,std::string)> onLocalIceCb_;
    std::function<void()> onDcOpenCb_;
    std::function<void(const std::string&)> onDcMessageCb_;

    /* State */
    bool dcOpen_{false};
    bool haveRemoteOffer_{false};
    bool localAnswerSent_{false};

    /* ICE buffer */
    std::vector<rtc::Candidate> iceBuffer_;

    /*FIX: cache SDP if callback registered late */
    std::optional<std::string> cachedAnswer_;
};
