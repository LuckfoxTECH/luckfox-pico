// transport/ITransport.hpp
#pragma once
#include <cstdint>
#include <vector>
#include <functional>

using u8 = uint8_t;

/**
 * @brief Transport RX callback type
 */
using TransportRxCallback = std::function<void(const std::vector<u8>&)>;

/**
 * @brief Transport interface (AUTOSAR-like)
 */
class ITransport
{
public:
    virtual ~ITransport() = default;

    /**
     * @brief Send binary data
     */
    virtual void send(const std::vector<u8>& data) = 0;

    /**
     * @brief Register RX callback
     */
    virtual void set_rx_callback(TransportRxCallback cb) = 0;

    /**
     * @brief Transport status
     */
    virtual bool is_connected() const = 0;
};
