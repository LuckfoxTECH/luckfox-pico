#include "WebRTCTransport.hpp"
#include <iostream>

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

        // ---- Prefer UDP ----
    cfg.iceTransportPolicy = rtc::TransportPolicy::All;
    cfg.enableIceTcp = false;

    pc_ = std::make_shared<rtc::PeerConnection>(cfg);

    /* ================= Local SDP ================= */

    pc_->onLocalDescription([this](rtc::Description d) {
        std::string sdp = std::string(d);

        if (onLocalSdpCb_)
            onLocalSdpCb_(sdp);
        else
            cachedSdp_ = sdp;
    });

    /* ================= Local ICE ================= */

    pc_->onLocalCandidate([this](rtc::Candidate c) {
        if (onLocalIceCb_)
            onLocalIceCb_(c.candidate(), c.mid());
    });

    /* ================= DataChannel ================= */
    rtc::DataChannelInit dc_cfg{};
    dc_cfg.protocol = "control";
    dc_cfg.negotiated = false;

    // ---- Reliability config ----
    rtc::Reliability r;
    r.unordered = true;              
    r.maxRetransmits = 0;           

    dc_cfg.reliability = r;

    // ---- Create channel ----
    dc_ = pc_->createDataChannel("dc", dc_cfg);


    dc_->onOpen([this] {
        dcOpen_.store(true);
        lastPing_ = lastPong_ = steady_clock::now();

        std::cout << "[DC] open\n";
        if (onDcOpenCb_) onDcOpenCb_();
    });

    dc_->onClosed([this] {
        dcOpen_.store(false);

        std::cout << "[DC] closed\n";
        if (onDcClosedCb_) onDcClosedCb_();
    });

    dc_->onMessage([this](rtc::message_variant msg) {

        /* ================= TEXT ================= */
        if (auto* text = std::get_if<std::string>(&msg)) {

            if (*text == "__ping__") {
                dc_->send("__pong__");
            }
            else if (*text == "__pong__") {
                lastPong_ = steady_clock::now();
            }
            else if (onDcMessageCb_) {
                onDcMessageCb_(*text);
            }
        }
        /* ================= BINARY ================= */
        else if (auto* bin = std::get_if<rtc::binary>(&msg)) {

            if (onDcBinaryCb_) {
                std::vector<uint8_t> data;
                data.reserve(bin->size());

                for (std::byte b : *bin) {
                    data.push_back(
                        static_cast<uint8_t>(b)
                    );
                }

                onDcBinaryCb_(data);
            }
        }
    });

    /* ================= ICE state ================= */

    pc_->onIceStateChange(
        [this](rtc::PeerConnection::IceState s)
        {
            std::cout << "[ICE] state="
                      << static_cast<int>(s) << "\n";

            if (s == rtc::PeerConnection::IceState::Failed ||
                s == rtc::PeerConnection::IceState::Disconnected)
            {
                std::cout << "[ICE] FAILED → reset\n";
                close();
            }
        });
}

void WebRTCTransport::destroyPeerConnection()
{
    try {
        if (dc_) dc_->close();
        if (pc_) pc_->close();
    } catch (...) {}

    dc_.reset();
    pc_.reset();

    dcOpen_.store(false);
    haveRemoteAnswer_ = false;

    iceBuffer_.clear();
    cachedSdp_.reset();
}

/* ============================================================
 * Callback registration
 * ============================================================ */

void WebRTCTransport::onLocalSdp(
    std::function<void(std::string)> cb)
{
    onLocalSdpCb_ = std::move(cb);

    if (cachedSdp_) {
        onLocalSdpCb_(*cachedSdp_);
        cachedSdp_.reset();
    }
}

void WebRTCTransport::onLocalIce(
    std::function<void(std::string, std::string)> cb)
{
    onLocalIceCb_ = std::move(cb);
}

void WebRTCTransport::onDcOpen(std::function<void()> cb)
{
    onDcOpenCb_ = std::move(cb);
}

void WebRTCTransport::onDcClosed(std::function<void()> cb)
{
    onDcClosedCb_ = std::move(cb);
}

void WebRTCTransport::onDcMessage(
    std::function<void(const std::string&)> cb)
{
    onDcMessageCb_ = std::move(cb);
}

void WebRTCTransport::onDcBinary(
    std::function<void(const std::vector<uint8_t>&)> cb)
{
    onDcBinaryCb_ = std::move(cb);
}

/* ============================================================
 * Signaling
 * ============================================================ */

void WebRTCTransport::createOffer()
{
    if (pc_)
        pc_->setLocalDescription(
            rtc::Description::Type::Offer);
}

void WebRTCTransport::setRemoteAnswer(const std::string& sdp)
{
    if (!pc_) return;

    if (pc_->signalingState() !=
        rtc::PeerConnection::SignalingState::HaveLocalOffer)
    {
        std::cout << "[SDP] Ignore ANSWER (state != HaveLocalOffer)\n";
        return;
    }

    pc_->setRemoteDescription(
        rtc::Description(sdp, rtc::Description::Type::Answer));

    haveRemoteAnswer_ = true;

    for (auto& c : iceBuffer_)
        pc_->addRemoteCandidate(c);

    iceBuffer_.clear();
}

/* ============================================================
 * ICE
 * ============================================================ */

void WebRTCTransport::addRemoteIce(
    const std::string& cand,
    const std::string& mid)
{
    rtc::Candidate c(cand, mid);

    if (!pc_ || !haveRemoteAnswer_) {
        iceBuffer_.push_back(c);
        return;
    }

    pc_->addRemoteCandidate(c);
}

/* ============================================================
 * Data TX
 * ============================================================ */

bool WebRTCTransport::isReady() const
{
    return dcOpen_.load() && dc_ && dc_->isOpen();
}

void WebRTCTransport::sendMessage(
    const std::string& msg)
{
    if (isReady())
        dc_->send(msg);
}

void WebRTCTransport::sendBinary(
    const std::vector<uint8_t>& data)
{
    if (!isReady()) return;

    rtc::binary bin;
    bin.reserve(data.size());

    for (uint8_t b : data)
        bin.push_back(std::byte(b));

    dc_->send(bin);
}

/* ============================================================
 * Keepalive
 * ============================================================ */

void WebRTCTransport::tick()
{
    if (!isReady()) return;

    auto now = steady_clock::now();

    if (now - lastPing_ > seconds(3)) {
        dc_->send("__ping__");
        lastPing_ = now;
    }

    if (now - lastPong_ > seconds(8)) {
        std::cout << "[CONN] timeout → reset\n";
        close();
    }
}

/* ============================================================
 * Full reset
 * ============================================================ */

void WebRTCTransport::close()
{
    destroyPeerConnection();
    createPeerConnection();

    createOffer();  

    std::cout << "[Transport] session reset\n";
}

bool WebRTCTransport::haveLocalOffer() const
{
    if (!pc_)
        return false;

    return pc_->signalingState() ==
           rtc::PeerConnection::SignalingState::HaveLocalOffer;
}

