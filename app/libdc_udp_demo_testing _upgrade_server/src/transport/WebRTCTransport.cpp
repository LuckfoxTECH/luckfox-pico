#include "WebRTCTransport.hpp"
#include <iostream>
#include <mutex>

using namespace std::chrono;

/* ============================================================
 * Constructor
 * ============================================================ */

WebRTCTransport::WebRTCTransport()
{
    createPeerConnection();
}

/* ============================================================
 * PeerConnection lifecycle
 * ============================================================ */

void WebRTCTransport::createPeerConnection()
{
    rtc::Configuration cfg;
    cfg.iceServers.emplace_back("stun:stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(cfg);

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pc_ = pc;
    }

    /* ---------- Local SDP ---------- */
    pc->onLocalDescription([this](rtc::Description d) {
        if (d.type() != rtc::Description::Type::Answer)
            return;

        std::function<void(std::string)> cb;
        std::string sdp = std::string(d);

        {
            std::lock_guard<std::mutex> lock(mtx_);
            cb = onLocalSdpCb_;
            if (!cb)
                cachedAnswer_ = sdp;
        }

        if (cb)
            cb(sdp);
    });

    /* ---------- Local ICE ---------- */
    pc->onLocalCandidate([this](rtc::Candidate c) {
        std::function<void(std::string, std::string)> cb;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            cb = onLocalIceCb_;
        }
        if (cb)
            cb(c.candidate(), c.mid());
    });

    /* ---------- DataChannel ---------- */
    pc->onDataChannel([this](std::shared_ptr<rtc::DataChannel> dc) {

        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (dc_) {
                std::cout << "[DC] extra channel ignored\n";
                return;
            }
            dc_ = dc;
        }

        dc->onOpen([this] {
            std::function<void()> cb;
            {
                std::lock_guard<std::mutex> lock(mtx_);
                dcOpen_ = true;
                state_  = SessionState::Connected;
                lastPing_ = lastPong_ = steady_clock::now();
                cb = onDcOpenCb_;
            }
            if (cb) cb();
        });

        dc->onClosed([this] {
            std::function<void()> cb;
            {
                std::lock_guard<std::mutex> lock(mtx_);
                dcOpen_ = false;
                dc_.reset();
                state_ = SessionState::Closed;
                cb = onDcClosedCb_;
            }
            if (cb) cb();
        });

        dc->onMessage([this](rtc::message_variant msg) {

            if (auto* text = std::get_if<std::string>(&msg)) {

                if (*text == "__ping__") {
                    std::shared_ptr<rtc::DataChannel> dc;
                    {
                        std::lock_guard<std::mutex> lock(mtx_);
                        dc = dc_;
                    }
                    if (dc && dc->isOpen()) {
                        try { dc->send("__pong__"); } catch (...) {}
                    }
                }
                else if (*text == "__pong__") {
                    std::lock_guard<std::mutex> lock(mtx_);
                    lastPong_ = steady_clock::now();
                }
                else {
                    std::function<void(const std::string&)> cb;
                    {
                        std::lock_guard<std::mutex> lock(mtx_);
                        cb = onDcMessageCb_;
                    }
                    if (cb) cb(*text);
                }
            }
            else if (auto* bin = std::get_if<rtc::binary>(&msg)) {

                std::function<void(const std::vector<uint8_t>&)> cb;
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    cb = onDcBinaryCb_;
                }

                if (!cb) return;

                std::vector<uint8_t> data;
                data.reserve(bin->size());
                for (std::byte b : *bin)
                    data.push_back(std::to_integer<uint8_t>(b));

                cb(data);
            }
        });
    });

    /* ---------- ICE State ---------- */
    pc->onIceStateChange([this](rtc::PeerConnection::IceState s) {
        if (s == rtc::PeerConnection::IceState::Failed ||
            s == rtc::PeerConnection::IceState::Disconnected) {
            std::lock_guard<std::mutex> lock(mtx_);
            resetRequested_ = true;
        }
    });
}

void WebRTCTransport::destroyPeerConnection()
{
    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dc;

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pc = pc_;
        dc = dc_;
        pc_.reset();
        dc_.reset();
        dcOpen_ = false;
        haveRemoteOffer_ = false;
        iceBuffer_.clear();
        cachedAnswer_.reset();
    }

    try {
        if (dc) dc->close();
        if (pc) pc->close();
    } catch (...) {}
}

/* ============================================================
 * Callback registration
 * ============================================================ */

void WebRTCTransport::onLocalSdp(std::function<void(std::string)> cb)
{
    std::optional<std::string> cached;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        onLocalSdpCb_ = cb;
        cached = cachedAnswer_;
        cachedAnswer_.reset();
    }
    if (cached && cb)
        cb(*cached);
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string, std::string)> cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    onLocalIceCb_ = std::move(cb);
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    onDcOpenCb_ = std::move(cb);
}

void WebRTCTransport::onDcClosed(std::function<void()> cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    onDcClosedCb_ = std::move(cb);
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    onDcMessageCb_ = std::move(cb);
}

void WebRTCTransport::onDcBinary(
    std::function<void(const std::vector<uint8_t>&)> cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    onDcBinaryCb_ = std::move(cb);
}

/* ============================================================
 * Signaling
 * ============================================================ */

void WebRTCTransport::setRemoteOffer(const std::string& sdp)
{
    destroyPeerConnection();
    createPeerConnection();

    std::shared_ptr<rtc::PeerConnection> pc;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pc = pc_;
        haveRemoteOffer_ = true;
        state_ = SessionState::Connecting;
    }

    pc->setRemoteDescription(
        rtc::Description(sdp, rtc::Description::Type::Offer));

    for (auto& c : iceBuffer_)
        pc->addRemoteCandidate(c);
    iceBuffer_.clear();

    createAnswer();
}

void WebRTCTransport::createAnswer()
{
    std::shared_ptr<rtc::PeerConnection> pc;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pc = pc_;
    }

    if (pc &&
        pc->signalingState() ==
        rtc::PeerConnection::SignalingState::HaveRemoteOffer) {
        pc->setLocalDescription(
            rtc::Description::Type::Answer);
    }
}

/* ============================================================
 * ICE
 * ============================================================ */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);

    std::shared_ptr<rtc::PeerConnection> pc;
    bool ready = false;

    {
        std::lock_guard<std::mutex> lock(mtx_);
        pc = pc_;
        ready = haveRemoteOffer_;
        if (!ready) {
            iceBuffer_.push_back(c);
            return;
        }
    }

    if (pc)
        pc->addRemoteCandidate(c);
}

/* ============================================================
 * Keepalive / watchdog
 * ============================================================ */

void WebRTCTransport::tick()
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (state_ == SessionState::Idle ||
            state_ == SessionState::Closed)
            return;
    }

    bool doReset = false;
    std::shared_ptr<rtc::DataChannel> dc;

    {
        std::lock_guard<std::mutex> lock(mtx_);

        if (resetRequested_) {
            resetRequested_ = false;
            doReset = true;
        }

        if (!dcOpen_ || !dc_)
            goto out;

        dc = dc_;

        auto now = steady_clock::now();

        if (now - lastPing_ > seconds(3)) {
            lastPing_ = now;
            if (dc->isOpen()) {
                try { dc->send("__ping__"); } catch (...) {}
            }
        }

        if (now - lastPong_ >= seconds(20))
            doReset = true;
    }

out:
    if (doReset) {
        std::cout << "[Transport] reset\n";
        close();
    }
}

/* ============================================================
 * Data send
 * ============================================================ */

void WebRTCTransport::sendMessage(const std::string& msg)
{
    std::shared_ptr<rtc::DataChannel> dc;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        dc = dc_;
    }

    if (dc && dc->isOpen())
        dc->send(msg);
}

void WebRTCTransport::sendBinary(
    const std::vector<uint8_t>& data)
{
    std::shared_ptr<rtc::DataChannel> dc;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        dc = dc_;
    }

    if (!dc || !dc->isOpen())
        return;

    rtc::binary bin;
    bin.reserve(data.size());
    for (uint8_t b : data)
        bin.push_back(std::byte(b));

    dc->send(bin);
}

/* ============================================================
 * Full reset (WAIT FOR NEW OFFER)
 * ============================================================ */

void WebRTCTransport::close()
{
    destroyPeerConnection();

    {
        std::lock_guard<std::mutex> lock(mtx_);
        state_ = SessionState::Idle;
    }

    std::cout << "[Transport] session closed, waiting for new offer\n";
}
