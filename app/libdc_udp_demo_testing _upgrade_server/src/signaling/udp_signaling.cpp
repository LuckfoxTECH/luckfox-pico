#include "udp_signaling.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <unistd.h>

UdpSignaling::UdpSignaling() noexcept
    : sock_(-1),
      running_(false),
      hasRemote_(false)
{
    std::memset(&localAddr_, 0, sizeof(localAddr_));
    std::memset(&remoteAddr_, 0, sizeof(remoteAddr_));
}

UdpSignaling::~UdpSignaling() noexcept
{
    stop();
}

UdpSignaling::Result
UdpSignaling::init(const char* localIp,
                   std::uint16_t localPort,
                   const char* remoteIp,
                   std::uint16_t remotePort) noexcept
{
    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_ < 0)
        return Result::Error;

    localAddr_.sin_family = AF_INET;
    localAddr_.sin_port   = htons(localPort);
    localAddr_.sin_addr.s_addr =
        localIp ? inet_addr(localIp) : INADDR_ANY;

    if (bind(sock_,
             (sockaddr*)&localAddr_,
             sizeof(localAddr_)) < 0)
        return Result::Error;

    if (remoteIp)
    {
        remoteAddr_.sin_family = AF_INET;
        remoteAddr_.sin_port   = htons(remotePort);
        remoteAddr_.sin_addr.s_addr = inet_addr(remoteIp);
        hasRemote_ = true;
    }

    return Result::Ok;
}

UdpSignaling::Result
UdpSignaling::start() noexcept
{
    running_ = true;
    std::thread(&UdpSignaling::recvLoop, this).detach();
    return Result::Ok;
}

UdpSignaling::Result
UdpSignaling::stop() noexcept
{
    running_ = false;

    if (sock_ >= 0)
    {
        close(sock_);
        sock_ = -1;
    }
    return Result::Ok;
}

void
UdpSignaling::setReceiveCallback(RxCallback cb) noexcept
{
    rxCb_ = cb;
}

UdpSignaling::Result
UdpSignaling::send(const std::uint8_t* data,
                   std::size_t len) noexcept
{
    if (!hasRemote_)
        return Result::Error;

    sendto(sock_,
           data,
           len,
           0,
           (sockaddr*)&remoteAddr_,
           sizeof(remoteAddr_));
    return Result::Ok;
}

void
UdpSignaling::recvLoop() noexcept
{
    std::uint8_t buf[2048];

    while (running_)
    {
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);

        ssize_t n = recvfrom(sock_,
                             buf,
                             sizeof(buf),
                             0,
                             (sockaddr*)&from,
                             &fromLen);
        if (n <= 0)
            continue;

        remoteAddr_ = from;
        hasRemote_  = true;

        if (rxCb_)
        {
            rxCb_(buf,
                  static_cast<std::size_t>(n),
                  ntohl(from.sin_addr.s_addr),
                  ntohs(from.sin_port));
        }
    }
}
