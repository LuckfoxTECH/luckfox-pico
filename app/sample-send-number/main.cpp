#include <rtc/rtc.hpp>
#include <iostream>
#include <string> 
#include <thread>
#include <chrono>

int main() {
    // 1. Khởi tạo logger
    rtc::InitLogger(rtc::LogLevel::Info);

    // 2. Khởi tạo PeerConnection
    rtc::Configuration config;
    auto pc = std::make_shared<rtc::PeerConnection>(config);
    
    // 3. Khởi tạo DataChannel
    auto dc = pc->createDataChannel("test");

    // 4. Thiết lập callback DataChannel
    dc->onMessage([](rtc::message_variant data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "Received: " << std::get<std::string>(data) << std::endl;
        }
    });

    dc->onOpen([dc]() {
        std::cout << "DataChannel open → sending number 1\n";
        dc->send("1");
    });
    
    // Sử dụng GatheringState như đã sửa ở lần trước
    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            std::cout << "ICE Gathering Complete.\n";
        }
    });

    // 5. SỬA LỖI: Khởi tạo rtc::LocalDescriptionInit
    try {
        rtc::Description offer = pc->createOffer();
        
        std::cout << "\n===== LOCAL SDP (OFFER) =====\n";
        std::cout << std::string(offer) << std::endl; 
        std::cout << "=============================\n\n";

        // SỬA: Thay đổi cú pháp khởi tạo để tránh Most Vexing Parse.
        // Sử dụng Uniform Initialization (dấu ngoặc nhọn {}) hoặc cú pháp khởi tạo bằng (=).
        rtc::LocalDescriptionInit init{std::string(offer)}; // Khởi tạo đồng nhất (Uniform Initialization)
        
        // Gọi hàm với hai đối số Type và đối tượng Init
        pc->setLocalDescription(offer.type(), init); 
        
        std::cout << "Local description set.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating or setting local description: " << e.what() << "\n";
    }

    // 6. Nhận Remote SDP
    std::cout << "Paste remote SDP (ANSWER) and press Enter:\n";
    std::string remote;
    std::string remoteLine;
    
    // Đọc nhiều dòng cho SDP
    while (std::getline(std::cin, remoteLine) && !remoteLine.empty()) {
        remote += remoteLine + "\n";
    }

    if (!remote.empty()) {
        // 7. setRemoteDescription (đã sửa ở lần trước)
        try {
            // Khởi tạo rtc::Description với type là "answer"
            rtc::Description remoteDesc(remote, "answer");
            
            // Chỉ truyền đối tượng rtc::Description hoàn chỉnh.
            pc->setRemoteDescription(remoteDesc); 
            
            std::cout << "Remote description set.\n";
        } catch (const std::exception& e) {
            std::cerr << "Error setting remote description: " << e.what() << "\n";
        }
    } else {
        std::cerr << "Error: Remote SDP is empty.\n";
    }

    // 8. Vòng lặp giữ chương trình chạy
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
