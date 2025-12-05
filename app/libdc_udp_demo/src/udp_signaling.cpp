#include "udp_signaling.hpp"
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

UdpSignaling::UdpSignaling(const std::string& local_ip, int local_port,
                           const std::string& remote_ip, int remote_port)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        perror("socket");
        throw std::runtime_error("Cannot create UDP socket");
    }

    std::memset(&local_addr_, 0, sizeof(local_addr_));
    local_addr_.sin_family = AF_INET;
    local_addr_.sin_port = htons(local_port);
    if (local_ip.empty()) {
        local_addr_.sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_pton(AF_INET, local_ip.c_str(), &local_addr_.sin_addr) != 1) {
            close(sockfd_);
            throw std::runtime_error("Invalid local IP");
        }
    }

    if (bind(sockfd_, (sockaddr*)&local_addr_, sizeof(local_addr_)) < 0) {
        perror("bind");
        close(sockfd_);
        throw std::runtime_error("Cannot bind UDP socket");
    }

    if (!remote_ip.empty() && remote_port > 0) {
        std::memset(&remote_addr_, 0, sizeof(remote_addr_));
        remote_addr_.sin_family = AF_INET;
        remote_addr_.sin_port = htons(remote_port);
        if (inet_pton(AF_INET, remote_ip.c_str(), &remote_addr_.sin_addr) != 1) {
            close(sockfd_);
            throw std::runtime_error("Invalid remote IP");
        }
        remote_set_ = true;
    }
}

UdpSignaling::~UdpSignaling() {
    stop();
    if (sockfd_ >= 0) close(sockfd_);
}

void UdpSignaling::start() {
    running_ = true;
    recv_thread_ = std::thread([this]() { receiveLoop(); });
}

void UdpSignaling::stop() {
    running_ = false;
    if (recv_thread_.joinable()) recv_thread_.join();
}

void UdpSignaling::send(const std::string& msg) {
    std::lock_guard<std::mutex> lk(send_mutex_);
    if (!remote_set_) {
        std::cerr << "[UdpSignaling] remote not set, cannot send\n";
        return;
    }
    ssize_t n = sendto(sockfd_, msg.c_str(), msg.size(), 0,
                       (sockaddr*)&remote_addr_, sizeof(remote_addr_));
    if (n < 0) perror("sendto");
}

void UdpSignaling::onMessage(std::function<void(const std::string&, const std::string&, int)> cb) {
    message_callback_ = cb;
}

void UdpSignaling::setRemote(const std::string& remote_ip, int remote_port) {
    std::memset(&remote_addr_, 0, sizeof(remote_addr_));
    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_port = htons(remote_port);
    if (inet_pton(AF_INET, remote_ip.c_str(), &remote_addr_.sin_addr) != 1) {
        throw std::runtime_error("Invalid remote ip");
    }
    remote_set_ = true;
}

void UdpSignaling::receiveLoop() {
    char buf[65536];
    while (running_) {
        sockaddr_in src{};
        socklen_t srclen = sizeof(src);
        ssize_t n = recvfrom(sockfd_, buf, sizeof(buf)-1, 0, (sockaddr*)&src, &srclen);
        if (n > 0) {
            buf[n] = '\0';
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &src.sin_addr, ipstr, sizeof(ipstr));
            int port = ntohs(src.sin_port);

            // if remote not set, learn it (server mode)
            if (!remote_set_) {
                remote_addr_ = src;
                remote_set_ = true;
            }

            if (message_callback_) {
                message_callback_(std::string(buf, n), std::string(ipstr), port);
            }
        } else {
            // small sleep to avoid busy-loop on some errors
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
