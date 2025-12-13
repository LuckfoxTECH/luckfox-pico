#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <cstring>
#include <algorithm>
#include "udp_signaling.hpp"
#include "rtc/rtc.hpp"

//--------------------------------------------------
// ENUM COMMAND
//--------------------------------------------------
typedef enum {
    CMD_NONE            = 0x00,
    CMD_PING            = 0x01,
    CMD_GET_STATUS      = 0x02,
    CMD_GET_SENSOR      = 0x03,

    CMD_SET_SPEED       = 0x10,
    CMD_SET_DIRECTION   = 0x11,
    CMD_SET_STEERING    = 0x12,
    CMD_SET_BRAKE       = 0x13,

    CMD_START_MOTOR     = 0x20,
    CMD_STOP_MOTOR      = 0x21,

    CMD_MAX
} Command_t;

// ACK
typedef enum {
    ACK_OK              = 0x00,
    ACK_INVALID_CMD     = 0x01,
    ACK_INVALID_PAYLOAD = 0x02,
    ACK_CRC_ERROR       = 0x03,
    ACK_BUSY            = 0x04,
    ACK_FAIL            = 0x05
} AckCode_t;

//--------------------------------------------------
// Frame Struct
//--------------------------------------------------
typedef struct {
    uint8_t sof;
    uint8_t msg_id;
    uint8_t cmd;
    uint8_t length;
    uint8_t payload[256];
    uint8_t checksum;
} Frame_t;

//--------------------------------------------------
// Checksum
//--------------------------------------------------
uint8_t calc_checksum(const Frame_t& f) {
    uint8_t sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;
    for (int i = 0; i < f.length; i++)
        sum ^= f.payload[i];
    return sum;
}

//--------------------------------------------------
// Build ACK frame
//--------------------------------------------------
std::vector<uint8_t> build_frame(uint8_t msg_id, uint8_t cmd, const uint8_t* payload, uint8_t len) {
    Frame_t f{};
    f.sof = 0xAA;
    f.msg_id = msg_id;
    f.cmd = cmd;
    f.length = len;

    if (len > 0 && payload != nullptr)
        memcpy(f.payload, payload, len);

    f.checksum = calc_checksum(f);

    std::vector<uint8_t> out;
    out.reserve(4 + len + 1);
    out.push_back(f.sof);
    out.push_back(f.msg_id);
    out.push_back(f.cmd);
    out.push_back(f.length);
    for (int i = 0; i < len; i++)
        out.push_back(f.payload[i]);
    out.push_back(f.checksum);

    return out;
}

//--------------------------------------------------
// Parse frame
//--------------------------------------------------
bool parse_frame(const uint8_t* buf, size_t size, Frame_t& out) {
    if (size < 5) return false;
    if (buf[0] != 0xAA) return false;

    out.sof    = buf[0];
    out.msg_id = buf[1];
    out.cmd    = buf[2];
    out.length = buf[3];

    if (4 + out.length + 1 != size)
        return false;

    memcpy(out.payload, &buf[4], out.length);
    out.checksum = buf[4 + out.length];

    return out.checksum == calc_checksum(out);
}

//--------------------------------------------------
// Base64
//--------------------------------------------------
static const std::string b64_table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64_table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back(b64_table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string &in) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[(unsigned char)b64_table[i]] = i;
    
    std::string out;
    int val = 0, valb = -8;

    for (unsigned char c : in) {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

//--------------------------------------------------
// Helper: convert vector<uint8_t> -> rtc::binary (vector<std::byte>)
//--------------------------------------------------
static rtc::binary vec_u8_to_rtcbin(const std::vector<uint8_t>& v) {
    rtc::binary out(v.size());
    for (size_t i = 0; i < v.size(); ++i) out[i] = std::byte(v[i]);
    return out;
}

//--------------------------------------------------
// MAIN SERVER
//--------------------------------------------------
int main() {
    const int SERVER_PORT = 6000;

    std::cout << "[Server] UDP Signaling on port " << SERVER_PORT << std::endl;

    UdpSignaling signaling("", SERVER_PORT);
    signaling.start();

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(config);

    // Send local SDP
    pc->onLocalDescription([&signaling](rtc::Description desc) {
        std::string msg = "SDP|" + base64_encode(std::string(desc));
        signaling.send(msg);
        std::cout << "[Server] Sent SDP\n";
    });

    // Send ICE
    pc->onLocalCandidate([&signaling](rtc::Candidate cand) {
        std::string msg = "ICE|" + base64_encode(cand.candidate())
                        + "|" + base64_encode(cand.mid());
        signaling.send(msg);
        std::cout << "[Server] Sent ICE\n";
    });

    std::shared_ptr<rtc::DataChannel> dc_remote;

    //--------------------------------------------------
    // DataChannel RECEIVED from client
    //--------------------------------------------------
    pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> dc) {
        std::cout << "[Server] DataChannel opened: " << dc->label() << "\n";
        dc_remote = dc;

        // When client sends a frame
        dc->onMessage([dc](rtc::message_variant msg) {
            if (!std::holds_alternative<rtc::binary>(msg)) {
                std::cout << "[Server] Non-binary message ignored\n";
                return;
            }

            auto bin = std::get<rtc::binary>(msg);

            Frame_t f{};
            bool ok = parse_frame(
                reinterpret_cast<const uint8_t*>(bin.data()),
                bin.size(),
                f
            );

            if (!ok) {
                std::cout << "[Server] Invalid frame CRC\n";
                return;
            }

            std::cout << "[Server] Received CMD=" << (int)f.cmd
                      << " msg_id=" << (int)f.msg_id
                      << " len=" << (int)f.length << "\n";

            //--------------------------------------------------
            // Build ACK and send back
            //--------------------------------------------------
            uint8_t ack_payload[1] = { ACK_OK };
            auto ack = build_frame(f.msg_id, f.cmd, ack_payload, 1);

            // convert to rtc::binary and send
            rtc::binary bin_ack = vec_u8_to_rtcbin(ack);
            try {
                dc->send(bin_ack);
                std::cout << "[Server] Sent ACK for cmd=" << (int)f.cmd << "\n";
            } catch (const std::exception &e) {
                std::cout << "[Server] ACK send exception: " << e.what() << "\n";
            } catch (...) {
                std::cout << "[Server] ACK send unknown exception\n";
            }
        });
    });

    //--------------------------------------------------
    // Signaling via UDP
    //--------------------------------------------------
    signaling.onMessage([&](const std::string &msg, const std::string &ip, int port) {
        signaling.setRemote(ip, port);

        if (msg.rfind("SDP|", 0) == 0) {
            std::string sdp = base64_decode(msg.substr(4));
            pc->setRemoteDescription(rtc::Description(sdp));
            pc->setLocalDescription();     // generate answer
        }
        else if (msg.rfind("ICE|", 0) == 0) {
            size_t p = msg.find('|', 4);
            if (p != std::string::npos) {
                std::string c = base64_decode(msg.substr(4, p-4));
                std::string mid = base64_decode(msg.substr(p+1));
                pc->addRemoteCandidate(rtc::Candidate(c, mid));
            }
        }
    });

    std::cout << "[Server] Ready. Waiting for WebRTC client...\n";

    while (1) std::this_thread::sleep_for(std::chrono::seconds(1));
}
