#include "PeerServer.hpp"
#include <thread>
#include <chrono>
#include "com/FrameCodec.hpp"
#include "com/RxDispatcher.hpp"
#include <iostream>

PeerServer::PeerServer(SignalingManager& sig,
                       WebRTCTransport& rtc)
    : sig_(sig), rtc_(rtc)
{
    rtc_.onDcOpen([this]{
        std::cout << "[Server] DC OPEN\n";
        running_ = true;

        std::thread([this] {
            while (running_) {
                rtc_.tick();

                std::vector<uint8_t> bytes;

                // ---- Build frame ----
                bytes.push_back(0xAA); // SOF
                bytes.push_back(0x01); // MSG_ID
                bytes.push_back(0x10); // CMD = VEHICLE_STATE
                bytes.push_back(0x08); // LEN = 8 bytes payload

                // ---- Payload (8 bytes) ----
                // speed = 0x1122
                bytes.push_back(0x22); // speed LSB
                bytes.push_back(0x11); // speed MSB

                // rpm = 0x3344
                bytes.push_back(0x44); // rpm LSB
                bytes.push_back(0x33); // rpm MSB

                // fault_flags
                bytes.push_back(0x55); // fault_flags

                // padding bytes (compiler padding)
                bytes.push_back(0x00);
                bytes.push_back(0x00);
                bytes.push_back(0x00);

                // ---- Calculate checksum ----
                uint8_t checksum = 0;
                for (auto b : bytes)
                    checksum ^= b;

                bytes.push_back(checksum);

                // ---- Send ----
                rtc_.sendBinary(bytes);

                std::this_thread::sleep_for(
                    std::chrono::seconds(1));
            }
        }).detach();


    });

    rtc_.onDcClosed([this]{
        std::cout << "[Server] DC CLOSED\n";
        running_ = false;
    });

    rtc_.onDcMessage([](const std::string& msg){
        std::cout << "[Server] DC msg: " << msg << "\n";
    });

    rtc_.onDcBinary([this](const std::vector<uint8_t>& data) {

        // -------- Basic guard --------
        // std::cout << "[RX RAW] (" << data.size() << " bytes): ";
        // for (auto b : data)
        //     printf("%02X ", b);
        // printf("\n");

        if (data.size() < 5) {
            std::cout << "[COM] RX frame too short\n";
            return;
        }

        Frame frame{};
        if (!FrameCodec::decode_frame(data.data(), data.size(), frame)) {
            std::cout << "[COM] RX invalid frame (checksum / format)\n";
            return;
        }

        // -------- Dispatch to COM RX --------
        RxDispatcher::dispatch(frame);
    });

    rtc_.onLocalSdp([this](std::string sdp){
        sig_.sendSdp(sdp);
    });

    rtc_.onLocalIce([this](std::string c, std::string m){
        sig_.sendIce(c, m);
    });

    sig_.onReceive([this](const SignalingMsg& m){
        if (m.type == SigType::SDP)
            rtc_.setRemoteOffer(m.payload1);
        else if (m.type == SigType::ICE)
            rtc_.addRemoteIce(m.payload1, m.payload2);
    });
}

void PeerServer::start()
{
    sig_.start();
}
