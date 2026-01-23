#include <rtc/rtc.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

extern "C" {

// Hàm khởi động module WebRTC
void start_webrtc() {
    rtc::InitLogger(rtc::LogLevel::Info);

    rtc::Configuration config;

    // Nếu dùng Tailscale, không cần STUN/TURN
    auto pc = std::make_shared<rtc::PeerConnection>(config);
    auto dc = pc->createDataChannel("echo");

    // Khi DataChannel mở
    dc->onOpen([dc]() {
        std::cout << "[MODULE] DataChannel opened\n";
    });

    // Nhận message từ DataChannel
    dc->onMessage([dc](rtc::message_variant message) {
        std::visit([dc](auto&& msg){
            using T = std::decay_t<decltype(msg)>;
            if constexpr (std::is_same_v<T,std::string>) {
                std::cout << "[MODULE] Received (string): " << msg << std::endl;
                dc->send("Echo: " + msg);
            } else if constexpr (std::is_same_v<T,rtc::binary>) {
                std::string s(reinterpret_cast<const char*>(msg.data()), msg.size());
                std::cout << "[MODULE] Received (binary): " << s << std::endl;
                dc->send("Echo: " + s);
            }
        }, message);
    });

    // Khi SDP local được tạo
    pc->onLocalDescription([](rtc::Description desc) {
        std::cout << "----- MODULE SDP -----\n";
        std::cout << std::string(desc) << "\n";  // <--- sửa ở đây
        std::cout << "----------------------\n";
    });

    // Nhập SDP remote từ main app (hoặc file)
    std::cout << "[MODULE] Enter remote SDP (end with empty line):\n";
    std::string remoteLine;
    std::string remoteSDP;
    while (true) {
        std::getline(std::cin, remoteLine);
        if (remoteLine.empty()) break;
        remoteSDP += remoteLine + "\n";
    }
    pc->setRemoteDescription(remoteSDP);

    std::cout << "[MODULE] Running...\n";

    // Giữ module chạy, không CPU 100%
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // extern "C"