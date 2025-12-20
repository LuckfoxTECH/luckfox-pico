#pragma once
#include "ITransport.hpp"
#include <rtc/rtc.hpp>
#include <memory>
#include <atomic>

namespace transport {

class WebRTCTransport : public ITransport {
public:
    using DcOpenCallback = std::function<void()>;

    explicit WebRTCTransport(std::shared_ptr<rtc::DataChannel> dc);
    ~WebRTCTransport() override;

    // ITransport
    void send(const std::vector<u8>& data) override;
    void set_rx_callback(TransportRxCallback cb) override;
    bool is_connected() const override;

    // signaling hook
    void on_dc_open(DcOpenCallback cb);

private:
    void setup_callbacks();

    std::shared_ptr<rtc::DataChannel> dc_;
    TransportRxCallback rx_cb_;
    DcOpenCallback dc_open_cb_;

    std::atomic<bool> connected_{false};
};

} // namespace transport
