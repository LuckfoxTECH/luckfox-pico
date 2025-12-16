#pragma once
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <netinet/in.h>

class UdpSignaling {
public:
    // local_ip: nếu rỗng sẽ bind INADDR_ANY
    // remote_ip/remote_port có thể để rỗng/0 cho server (server sẽ learn từ packet đầu tiên)
    UdpSignaling(const std::string& local_ip, int local_port,
                 const std::string& remote_ip = "", int remote_port = 0);

    ~UdpSignaling();

    // start listener thread
    void start();

    // stop listener
    void stop();

    // send one datagram to remote (must have remote_set or call setRemote first)
    void send(const std::string& msg);

    // set callback: (message, src_ip, src_port)
    void onMessage(std::function<void(const std::string&, const std::string&, int)> cb);

    // set remote explicitly (client mode)
    void setRemote(const std::string& remote_ip, int remote_port);

private:
    void receiveLoop();

    int sockfd_;
    sockaddr_in local_addr_;
    sockaddr_in remote_addr_;
    bool remote_set_ = false;

    std::atomic<bool> running_{false};
    std::thread recv_thread_;
    std::mutex send_mutex_;

    std::function<void(const std::string&, const std::string&, int)> message_callback_;
};
