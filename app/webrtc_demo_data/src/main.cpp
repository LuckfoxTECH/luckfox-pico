#include <rtc/rtc.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

// Biến để lưu trữ và kiểm tra SDP Offer đã được tạo chưa
std::string localSdpOffer;

int main() {
    // 1. Initialize logger: Set to Info
    // Kích hoạt logger để xem các thông tin trạng thái chi tiết
    rtc::InitLogger(rtc::LogLevel::Info); 

    // 2. Create a PeerConnection with STUN server
    rtc::Configuration config;
    
    // Bỏ chú thích (uncomment) để thêm STUN server công cộng. 
    // Điều này RẤT quan trọng để kết nối qua Internet/NATs.
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    
    auto pc = std::make_shared<rtc::PeerConnection>(config);

    // 3. Create a DataChannel
    // DataChannel này sẽ được tạo tự động khi kết nối thành công (SCTP)
    auto dc = pc->createDataChannel("test");

    // 4. Setup DataChannel callbacks
    dc->onMessage([](rtc::message_variant data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "Received: " << std::get<std::string>(data) << std::endl;
        }
    });

    dc->onOpen([dc]() {
        std::cout << "✅ DataChannel open → sending number 1\n";
        // Gửi tin nhắn đầu tiên khi kênh mở
        dc->send("1"); 
    });
    
    // --- Troubleshooting Callbacks ---
    
    // 5. Monitor ICE gathering state
    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        if (state == rtc::PeerConnection::GatheringState::Complete) {
            std::cout << "🧊 ICE Gathering Complete.\n";
        }
    });

    // 6. Monitor ICE connection state
    pc->onStateChange([](rtc::PeerConnection::State state) {
        if (state == rtc::PeerConnection::State::Failed) {
            std::cerr << "❌ PeerConnection FAILED. Check network/STUN/TURN.\n";
        } else if (state == rtc::PeerConnection::State::Connected) {
            std::cout << "🔗 PeerConnection CONNECTED.\n";
        }
    });

    // 7. FIX: Use onLocalDescription to print the final SDP Offer
    // Callback này được gọi sau khi ICE Candidates đã được thu thập.
    pc->onLocalDescription([](rtc::Description localDesc) {
        if (localDesc.type() == rtc::Description::Type::Offer) {
            localSdpOffer = std::string(localDesc); // Lưu SDP
            
            // In ngay lập tức và flush buffer để hiển thị ngay lập tức
            std::cout << "\n============================================\n";
            std::cout << "===== LOCAL SDP (OFFER) TO SHARE =====\n";
            std::cout << "============================================\n";
            std::cout << localSdpOffer << std::endl; 
            std::cout << "============================================\n\n";
            std::cout << "Share this SDP (OFFER) with the remote peer.\n";
        }
    });

    // 8. Create an SDP offer and set as local description
    try {
        rtc::Description offer = pc->createOffer();

        // Sử dụng chữ ký API cũ hơn (Type + LocalDescriptionInit)
        rtc::LocalDescriptionInit init{std::string(offer)};
        pc->setLocalDescription(offer.type(), init);

        std::cout << "Local description set. Waiting for final SDP to be printed by callback...\n";
        
        // BƯỚC FIX QUAN TRỌNG: Chờ cho đến khi SDP được in ra (tối đa 10 giây)
        int count = 0;
        while (localSdpOffer.empty() && count < 100) {
            std::this_thread::sleep_for(1000ms);
            count++;
        }
        
        if (localSdpOffer.empty()) {
            std::cerr << "\n------------------------------------------------------\n";
            std::cerr << "❌ Error: SDP Offer was not generated or printed in time!\n";
            std::cerr << "------------------------------------------------------\n";
            // Không thoát, nhưng sẽ cảnh báo người dùng.
        }

    } catch (const std::exception& e) {
        std::cerr << "Error creating or setting local description: " << e.what() << "\n";
        return 1;
    }

    // 9. Read remote SDP (answer) from user input
    std::cout << "------------------------------------------------------\n";
    std::cout << "Paste remote SDP (ANSWER) and press Enter twice to finish:\n";
    std::string remote;
    std::string remoteLine;

    // Đọc input cho đến khi gặp dòng trống (Enter 2 lần)
    while (std::getline(std::cin, remoteLine) && !remoteLine.empty()) {
        remote += remoteLine + "\n";
    }

    if (!remote.empty()) {
        try {
            // Khởi tạo remote description với type "answer"
            rtc::Description remoteDesc(remote, "answer");

            // Thiết lập remote description. Hành động này bắt đầu ICE Checking.
            pc->setRemoteDescription(remoteDesc);

            std::cout << "Remote description set. Waiting for connection...\n";
        } catch (const std::exception& e) {
            std::cerr << "❌ Error setting remote description: " << e.what() << "\n";
        }
    } else {
        std::cerr << "❌ Error: Remote SDP is empty. Did you paste the Answer?\n";
    }

    // 10. Keep program running indefinitely
    std::cout << "Application running. Press Ctrl+C to exit.\n";
    while (true) {
        std::this_thread::sleep_for(1s);
    }
    
    return 0;
}