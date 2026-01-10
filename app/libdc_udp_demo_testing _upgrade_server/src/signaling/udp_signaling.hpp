#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <netinet/in.h>

class UdpSignaling
{
public:
    enum class Result
    {
        Ok,
        Error
    };

    using RxCallback =
        std::function<void(const std::uint8_t*,
                           std::size_t,
                           std::uint32_t,
                           std::uint16_t)>;

    UdpSignaling() noexcept;
    ~UdpSignaling() noexcept;

    Result init(const char* localIp,
                std::uint16_t localPort,
                const char* remoteIp,
                std::uint16_t remotePort) noexcept;

    Result start() noexcept;
    Result stop() noexcept;

    Result send(const std::uint8_t* data,
                std::size_t len) noexcept;

    void setReceiveCallback(RxCallback cb) noexcept;

private:
    int sock_;
    bool running_;

    sockaddr_in localAddr_;
    sockaddr_in remoteAddr_;
    bool hasRemote_;

    RxCallback rxCb_;

    void recvLoop() noexcept;
};
