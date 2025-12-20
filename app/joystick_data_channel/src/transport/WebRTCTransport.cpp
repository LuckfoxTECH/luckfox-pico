#include "WebRTCTransport.hpp"
#include <iostream>

namespace transport {

WebRTCTransport::WebRTCTransport(
    std::shared_ptr<rtc::DataChannel> dc)
    : dc_(std::move(dc))
{
    setup_callbacks();
}

WebRTCTransport::~WebRTCTransport()
{
    // libdatachannel tự cleanup callback
}

void WebRTCTransport::send(const std::vector<u8>& data)
{
    if (!dc_ || !connected_)
        return;

    rtc::binary bin(data.size());
    for (size_t i = 0; i < data.size(); ++i)
        bin[i] = std::byte(data[i]);

    dc_->send(bin);
}

void WebRTCTransport::set_rx_callback(TransportRxCallback cb)
{
    rx_cb_ = std::move(cb);
}

void WebRTCTransport::on_dc_open(DcOpenCallback cb)
{
    dc_open_cb_ = std::move(cb);
}

bool WebRTCTransport::is_connected() const
{
    return connected_;
}

// ============================================================
// CALLBACKS
// ============================================================

void WebRTCTransport::setup_callbacks()
{
    std::weak_ptr<rtc::DataChannel> weak_dc = dc_;

    dc_->onOpen([this, weak_dc]() {
        if (!weak_dc.lock())
            return;

        connected_ = true;
        std::cout << "[WebRTCTransport] DataChannel OPEN\n";

        if (dc_open_cb_)
            dc_open_cb_();
    });

    dc_->onClosed([this]() {
        connected_ = false;
        std::cout << "[WebRTCTransport] DataChannel CLOSED\n";
    });

    dc_->onMessage([this](rtc::message_variant msg) {
        if (!rx_cb_)
            return;

        if (!std::holds_alternative<rtc::binary>(msg))
            return;

        const auto& bin = std::get<rtc::binary>(msg);
        std::vector<u8> out(bin.size());

        for (size_t i = 0; i < bin.size(); ++i)
            out[i] = static_cast<u8>(bin[i]);

        rx_cb_(out);
    });
}

} // namespace transport
