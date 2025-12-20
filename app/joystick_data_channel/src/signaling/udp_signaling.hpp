#pragma once
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <netinet/in.h>

class UdpSignaling {
public:
    using RxCallback =
        std::function<void(const std::string& type,
                           const std::string& p1,
                           const std::string& p2)>;

    UdpSignaling(const std::string& local_ip, int local_port,
                 const std::string& remote_ip = "", int remote_port = 0);
    ~UdpSignaling();

    void start();
    void stop();

    // === TX API (HIGH LEVEL) ===
    void sendSdp(const std::string& sdp);
    void sendIce(const std::string& cand, const std::string& mid);

    // === RX callback (parsed) ===
    void onMessage(RxCallback cb);

private:
    void receiveLoop();

    // helpers
    static std::string base64_encode(const std::string& in);
    static std::string base64_decode(const std::string& in);

    int sockfd_{-1};
    sockaddr_in local_addr_{};
    sockaddr_in remote_addr_{};
    bool remote_set_{false};

    std::atomic<bool> running_{false};
    std::thread recv_thread_;
    std::mutex send_mutex_;

    RxCallback rx_cb_;
};
