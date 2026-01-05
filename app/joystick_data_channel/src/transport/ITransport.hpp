class ITransport {
public:
    virtual ~ITransport() = default;

    virtual bool isReady() const = 0;
    virtual void send(const std::vector<uint8_t>& data) = 0;
};
