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

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: peer_client <server_tailscale_ip> <server_signaling_port>\n";
        return 1;
    }
    std::string server_ip = argv[1];
    int server_port = std::stoi(argv[2]);

    const int CLIENT_PORT = 6001; // local port to receive signaling replies
    std::cout << "[Client] Server=" << server_ip << ":" << server_port << " local_port=" << CLIENT_PORT << std::endl;

    // client knows server IP:PORT
    UdpSignaling signaling("", CLIENT_PORT, server_ip, server_port);
    signaling.start();

    rtc::Configuration config;
    // STUN is optional when using Tailscale; leaving it is harmless
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onStateChange([](rtc::PeerConnection::State s){
        std::cout << "[Client] PC state: " << (int)s << std::endl;
    });

    // when local SDP created (offer), send to server via UDP
    pc->onLocalDescription([&signaling](rtc::Description desc) {
        std::string sdp = std::string(desc);
        std::string msg = "SDP|" + base64_encode(sdp);
        signaling.send(msg);
        std::cout << "[Client] Sent local SDP (" << msg.size() << " bytes)\n";
    });

    // when we have local ICE candidate, send to server
    pc->onLocalCandidate([&signaling](rtc::Candidate cand) {
        std::string payload = cand.candidate();
        std::string mid = cand.mid();
        std::string msg = "ICE|" + base64_encode(payload) + "|" + base64_encode(mid);
        signaling.send(msg);
        std::cout << "[Client] Sent ICE candidate\n";
    });

    // receive signaling messages from server
    signaling.onMessage([&](const std::string &msg, const std::string &src_ip, int src_port) {
        std::cout << "[Client] Received signaling from " << src_ip << ":" << src_port << " size=" << msg.size() << std::endl;
        if (msg.rfind("SDP|", 0) == 0) {
            std::string b = msg.substr(4);
            std::string sdp = base64_decode(b);
            pc->setRemoteDescription(rtc::Description(sdp));
            std::cout << "[Client] Set remote SDP\n";
        } else if (msg.rfind("ICE|", 0) == 0) {
            size_t p1 = msg.find('|', 4);
            if (p1 != std::string::npos) {
                std::string b1 = msg.substr(4, p1-4);
                std::string b2 = msg.substr(p1+1);
                std::string candidate = base64_decode(b1);
                std::string mid = base64_decode(b2);
                pc->addRemoteCandidate(rtc::Candidate(candidate, mid));
                std::cout << "[Client] Added remote ICE candidate\n";
            }
        } else {
            std::cout << "[Client] Unknown signaling msg\n";
        }
    });

    // Create DataChannel with default init so code works across libdatachannel versions
    rtc::DataChannelInit opts;  // use default init for maximum compatibility
    auto dc = pc->createDataChannel("ctrl", opts);

    dc->onOpen([dc](){
        std::cout << "[Client] DataChannel open\n";
    });

    dc->onMessage([dc](rtc::message_variant msg){
        if (std::holds_alternative<rtc::string>(msg)) {
            std::string s = std::get<rtc::string>(msg);
            std::cout << "[Client] Command received: " << s << std::endl;
            // fake execute and reply
            std::string reply = std::string("{\"status\":\"ok\",\"echo\":") + "\"" + s + "\"}";
            try {
                dc->send(reply);
                std::cout << "[Client] Sent reply\n";
            } catch (...) {
                std::cout << "[Client] reply send failed\n";
            }
        } else if (std::holds_alternative<rtc::binary>(msg)) {
            // handle binary if any
            auto bin = std::get<rtc::binary>(msg);
            std::cout << "[Client] Received binary message, len=" << bin.size() << std::endl;
        }
    });

    // client is offerer
    pc->setLocalDescription();

    // keep running
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    signaling.stop();
    return 0;
}
