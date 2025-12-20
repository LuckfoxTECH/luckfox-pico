#include "udp_signaling.hpp"
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// ============================================================
// Base64 (PRIVATE – transport detail)
// ============================================================

static const std::string b64 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string UdpSignaling::base64_encode(const std::string& in)
{
    std::string out;
    int val = 0, valb = -6;
    for (uint8_t c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back(b64[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string UdpSignaling::base64_decode(const std::string& in)
{
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[b64[i]] = i;

    std::string out;
    int val = 0, valb = -8;
    for (uint8_t c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

// ============================================================

UdpSignaling::UdpSignaling(const std::string& local_ip, int local_port,
                           const std::string& remote_ip, int remote_port)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) throw std::runtime_error("socket failed");

    local_addr_.sin_family = AF_INET;
    local_addr_.sin_port   = htons(local_port);
    local_addr_.sin_addr.s_addr =
        local_ip.empty() ? INADDR_ANY : inet_addr(local_ip.c_str());

    bind(sockfd_, (sockaddr*)&local_addr_, sizeof(local_addr_));

    if (!remote_ip.empty()) {
        remote_addr_.sin_family = AF_INET;
        remote_addr_.sin_port   = htons(remote_port);
        inet_pton(AF_INET, remote_ip.c_str(), &remote_addr_.sin_addr);
        remote_set_ = true;
    }
}

UdpSignaling::~UdpSignaling()
{
    stop();
    if (sockfd_ >= 0) close(sockfd_);
}

void UdpSignaling::start()
{
    running_ = true;
    recv_thread_ = std::thread(&UdpSignaling::receiveLoop, this);
}

void UdpSignaling::stop()
{
    running_ = false;
    if (recv_thread_.joinable())
        recv_thread_.join();
}

// ============================================================
// TX
// ============================================================

void UdpSignaling::sendSdp(const std::string& sdp)
{
    std::string msg = "SDP|" + base64_encode(sdp);
    std::lock_guard<std::mutex> lk(send_mutex_);
    sendto(sockfd_, msg.data(), msg.size(), 0,
           (sockaddr*)&remote_addr_, sizeof(remote_addr_));
}

void UdpSignaling::sendIce(const std::string& cand,
                           const std::string& mid)
{
    std::string msg =
        "ICE|" + base64_encode(cand) + "|" +
        base64_encode(mid);
    std::lock_guard<std::mutex> lk(send_mutex_);
    sendto(sockfd_, msg.data(), msg.size(), 0,
           (sockaddr*)&remote_addr_, sizeof(remote_addr_));
}

// ============================================================
// RX
// ============================================================

void UdpSignaling::onMessage(RxCallback cb)
{
    rx_cb_ = cb;
}

void UdpSignaling::receiveLoop()
{
    char buf[65536];

    while (running_) {
        sockaddr_in src{};
        socklen_t sl = sizeof(src);
        ssize_t n = recvfrom(sockfd_, buf, sizeof(buf)-1, 0,
                             (sockaddr*)&src, &sl);
        if (n <= 0) continue;
        buf[n] = 0;

        if (!remote_set_) {
            remote_addr_ = src;
            remote_set_ = true;
        }

        std::string msg(buf, n);

        // SDP|xxxx  or ICE|cand|mid
        if (!rx_cb_) continue;

        if (msg.rfind("SDP|", 0) == 0) {
            rx_cb_("SDP", base64_decode(msg.substr(4)), "");
        }
        else if (msg.rfind("ICE|", 0) == 0) {
            auto p = msg.find('|', 4);
            rx_cb_("ICE",
                   base64_decode(msg.substr(4, p-4)),
                   base64_decode(msg.substr(p+1)));
        }
    }
}
