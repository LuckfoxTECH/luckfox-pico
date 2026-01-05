#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <cstdint>

enum class SigType {
    SDP,
    ICE,
    ACK
};

struct SignalingMsg {
    SigType type;
    uint32_t seq;
    std::string payload1;
    std::string payload2;
};

class UdpSignaling; // forward

class SignalingManager {
public:
    using RxCallback = std::function<void(const SignalingMsg&)>;

    SignalingManager(UdpSignaling& udp,
                     const std::string& sessionId);

    void start();
    void stop();

    void sendSdp(const std::string& sdp);
    void sendIce(const std::string& cand,
                 const std::string& mid);

    void onReceive(RxCallback cb);

private:
    void handleUdp(const uint8_t* data, size_t len);
    void sendRaw(SigType type,
                 uint32_t seq,
                 const std::string& p1,
                 const std::string& p2 = "");

    void resendLoop();

    struct Pending {
        std::string raw;
        int retry;
    };

    UdpSignaling& udp_;
    std::string sessionId_;
    uint32_t seq_{1};

    std::unordered_map<uint32_t, Pending> pending_;
    RxCallback rxCb_;
    bool running_{false};
};
