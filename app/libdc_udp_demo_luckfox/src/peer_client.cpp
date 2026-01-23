#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "udp_signaling.hpp"
#include "rtc/rtc.hpp"

/* ================= IOCTL ================= */
#define SERVO_SET_ANGLE   _IOW('p', 1, int)
#define GPIO_SET_LEVEL    _IOW('p', 2, int)

/* ================= COMMAND ENUM ================= */
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
} Command_t;

/* ================= ACK ENUM ================= */
typedef enum {
    ACK_OK              = 0x00,
    ACK_INVALID_CMD     = 0x01,
    ACK_INVALID_PAYLOAD = 0x02,
    ACK_CRC_ERROR       = 0x03,
    ACK_BUSY            = 0x04,
    ACK_FAIL            = 0x05
} AckCode_t;

/* ================= FRAME ================= */
typedef struct {
    uint8_t sof;
    uint8_t msg_id;
    uint8_t cmd;
    uint8_t length;
    uint8_t payload[256];
    uint8_t checksum;
} Frame_t;

/* ================= CHECKSUM ================= */
uint8_t calc_checksum(const Frame_t& f) {
    uint8_t sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;
    for (int i = 0; i < f.length; i++)
        sum ^= f.payload[i];
    return sum;
}

/* ================= BUILD FRAME ================= */
std::vector<uint8_t> build_frame(uint8_t msg_id, uint8_t cmd,
                                 const uint8_t* payload, uint8_t len) {
    Frame_t f{};
    f.sof = 0xAA;
    f.msg_id = msg_id;
    f.cmd = cmd;
    f.length = len;

    if (len && payload)
        memcpy(f.payload, payload, len);

    f.checksum = calc_checksum(f);

    std::vector<uint8_t> out;
    out.reserve(5 + len);
    out.push_back(f.sof);
    out.push_back(f.msg_id);
    out.push_back(f.cmd);
    out.push_back(f.length);
    for (int i = 0; i < len; i++)
        out.push_back(f.payload[i]);
    out.push_back(f.checksum);

    return out;
}

/* ================= PARSE FRAME ================= */
bool parse_frame(const uint8_t* buf, size_t size, Frame_t& out) {
    if (size < 5) return false;
    if (buf[0] != 0xAA) return false;

    out.sof = buf[0];
    out.msg_id = buf[1];
    out.cmd = buf[2];
    out.length = buf[3];

    if (size != (size_t)(5 + out.length - 0))
        return false;

    memcpy(out.payload, &buf[4], out.length);
    out.checksum = buf[4 + out.length];

    return out.checksum == calc_checksum(out);
}

/* ================= BASE64 ================= */
static const std::string b64_table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const std::string &in) {
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

std::string base64_decode(const std::string &in) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[(unsigned char)b64_table[i]] = i;

    std::string out;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
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

/* ================= HELPER ================= */
static rtc::binary vec_u8_to_rtcbin(const std::vector<uint8_t>& v) {
    rtc::binary out(v.size());
    for (size_t i = 0; i < v.size(); i++)
        out[i] = std::byte(v[i]);
    return out;
}

/* ================= MAIN ================= */
int main() {
    const int SERVER_PORT = 6000;

    int ctrl_fd = open("/dev/luckfox_ctrl", O_RDWR);
    if (ctrl_fd < 0) {
        perror("open /dev/luckfox_ctrl");
        return -1;
    }

    std::cout << "[Server] UDP signaling on port " << SERVER_PORT << "\n";

    UdpSignaling signaling("", SERVER_PORT);
    signaling.start();

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onLocalDescription([&](rtc::Description desc) {
        signaling.send("SDP|" + base64_encode(std::string(desc)));
    });

    pc->onLocalCandidate([&](rtc::Candidate cand) {
        signaling.send("ICE|" + base64_encode(cand.candidate()) +
                       "|" + base64_encode(cand.mid()));
    });

    pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> dc) {
        std::cout << "[Server] DataChannel opened\n";

        dc->onMessage([dc, ctrl_fd](rtc::message_variant msg) {

            if (!std::holds_alternative<rtc::binary>(msg))
                return;

            auto bin = std::get<rtc::binary>(msg);
            Frame_t f{};

            uint8_t ack = ACK_OK;

            if (!parse_frame(reinterpret_cast<const uint8_t*>(bin.data()),
                             bin.size(), f)) {
                ack = ACK_CRC_ERROR;
            } else {
                switch (f.cmd) {

                case CMD_SET_STEERING: {
                    if (f.length < 1) {
                        ack = ACK_INVALID_PAYLOAD;
                        break;
                    }
                    int angle = std::clamp((int)f.payload[0], 0, 120);
                    if (ioctl(ctrl_fd, SERVO_SET_ANGLE, &angle) < 0) {
                        perror("SERVO_SET_ANGLE");
                        ack = ACK_FAIL;
                    }
                    break;
                }

                case CMD_START_MOTOR: {
                    int v = 1;
                    if (ioctl(ctrl_fd, GPIO_SET_LEVEL, &v) < 0)
                        ack = ACK_FAIL;
                    break;
                }

                case CMD_STOP_MOTOR: {
                    int v = 0;
                    if (ioctl(ctrl_fd, GPIO_SET_LEVEL, &v) < 0)
                        ack = ACK_FAIL;
                    break;
                }

                default:
                    ack = ACK_INVALID_CMD;
                    break;
                }
            }

            auto ack_frame = build_frame(f.msg_id, f.cmd, &ack, 1);
            dc->send(vec_u8_to_rtcbin(ack_frame));
        });
    });

    signaling.onMessage([&](const std::string &msg,
                            const std::string &ip, int port) {
        signaling.setRemote(ip, port);

        if (msg.rfind("SDP|", 0) == 0) {
            pc->setRemoteDescription(
                rtc::Description(base64_decode(msg.substr(4))));
            pc->setLocalDescription();
        }
        else if (msg.rfind("ICE|", 0) == 0) {
            size_t p = msg.find('|', 4);
            pc->addRemoteCandidate(
                rtc::Candidate(
                    base64_decode(msg.substr(4, p - 4)),
                    base64_decode(msg.substr(p + 1))));
        }
    });

    std::cout << "[Server] Ready\n";
    while (1) std::this_thread::sleep_for(std::chrono::seconds(1));
}

