#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include "udp_signaling.hpp"
#include "rtc/rtc.hpp"

// ---------- base64 helpers ----------
static const std::string b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64_table[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back(b64_table[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}
static std::string base64_decode(const std::string &in) {
    std::vector<int> T(256,-1);
    for (int i=0;i<64;i++) T[(unsigned char)b64_table[i]] = i;
    std::string out;
    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

int main() {
    const int SERVER_PORT = 6000;
    std::cout << "[Server] Starting UDP signaling on port " << SERVER_PORT << std::endl;

    // server: local_ip empty -> bind INADDR_ANY
    UdpSignaling signaling("", SERVER_PORT);
    signaling.start();

    rtc::Configuration config;
    // STUN optional when using Tailscale; harmless to keep
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onStateChange([](rtc::PeerConnection::State s) {
        std::cout << "[Server] PC state: " << (int)s << std::endl;
    });

    // send local SDP via UDP when created (base64)
    pc->onLocalDescription([&signaling](rtc::Description desc) {
        std::string sdp = std::string(desc);
        std::string msg = "SDP|" + base64_encode(sdp);
        signaling.send(msg);
        std::cout << "[Server] Sent local SDP (" << msg.size() << " bytes)\n";
    });

    // send local ICE
    pc->onLocalCandidate([&signaling](rtc::Candidate cand) {
        std::string payload = cand.candidate();
        std::string mid = cand.mid();
        std::string msg = "ICE|" + base64_encode(payload) + "|" + base64_encode(mid);
        signaling.send(msg);
        std::cout << "[Server] Sent ICE candidate\n";
    });

    std::shared_ptr<rtc::DataChannel> dc_remote;

    // when remote creates a DataChannel, set callbacks
    pc->onDataChannel([&dc_remote](std::shared_ptr<rtc::DataChannel> dc) {
        std::cout << "[Server] onDataChannel label=" << dc->label() << std::endl;
        dc_remote = dc;
        dc->onOpen([dc](){ std::cout << "[Server] DataChannel opened\n"; });
        dc->onMessage([dc](rtc::message_variant msg){
            if (std::holds_alternative<rtc::string>(msg)) {
                std::string s = std::get<rtc::string>(msg);
                std::cout << "[Server] Received from client: " << s << std::endl;
            }
        });
    });

    // signaling receive handler (from client). server will learn client ip on first packet automatically
    signaling.onMessage([&](const std::string &msg, const std::string &src_ip, int src_port){
        std::cout << "[Server] Signaling msg from " << src_ip << ":" << src_port << " (" << msg.size() << " bytes)\n";
        // set remote explicitly (optional) so future sends use that endpoint
        signaling.setRemote(src_ip, src_port);

        if (msg.rfind("SDP|", 0) == 0) {
            std::string b = msg.substr(4);
            std::string sdp = base64_decode(b);
            pc->setRemoteDescription(rtc::Description(sdp));
            pc->setLocalDescription(); // create answer
            std::cout << "[Server] Set remote SDP and created local answer\n";
        } else if (msg.rfind("ICE|", 0) == 0) {
            size_t p1 = msg.find('|', 4);
            if (p1 != std::string::npos) {
                std::string b1 = msg.substr(4, p1-4);
                std::string b2 = msg.substr(p1+1);
                std::string candidate = base64_decode(b1);
                std::string mid = base64_decode(b2);
                pc->addRemoteCandidate(rtc::Candidate(candidate, mid));
                std::cout << "[Server] Added remote ICE candidate\n";
            }
        } else {
            std::cout << "[Server] Unknown signaling msg\n";
        }
    });

    // background thread: periodically send control commands when DC open
    std::atomic<bool> running{true};
    std::thread sender([&](){
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (dc_remote && dc_remote->isOpen()) {
                std::string cmd = R"({"cmd":"PING","ts":)" + std::to_string(time(nullptr)) + "}";
                try {
                    dc_remote->send(cmd);
                    std::cout << "[Server] Sent command: " << cmd << std::endl;
                } catch (...) {
                    std::cout << "[Server] send failed\n";
                }
            }
        }
    });

    std::cout << "[Server] Ready. Waiting for client signaling..." << std::endl;

    // keep main alive
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    running = false;
    sender.join();
    signaling.stop();
    return 0;
}

