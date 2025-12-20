#pragma once
#include "ITransport.hpp"

class LoopbackTransport : public ITransport
{
public:
    LoopbackTransport() = default;

    void send(const std::vector<u8>& data) override;
    void set_rx_callback(TransportRxCallback cb) override;
    bool is_connected() const override;

private:
    TransportRxCallback rx_cb_{};
};
