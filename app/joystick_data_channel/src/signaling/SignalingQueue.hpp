#pragma once
#include <atomic>
#include <array>
namespace signaling {

/**
 * @brief Single Producer – Single Consumer queue
 */
template<typename T, size_t N>
class SignalingQueue
{
public:
    bool push(const T& v)
    {
        size_t h = head_.load(std::memory_order_relaxed);
        size_t n = (h + 1) % N;
        if (n == tail_.load(std::memory_order_acquire))
            return false;

        buf_[h] = v;
        head_.store(n, std::memory_order_release);
        return true;
    }

    bool pop(T& out)
    {
        size_t t = tail_.load(std::memory_order_relaxed);
        if (t == head_.load(std::memory_order_acquire))
            return false;

        out = buf_[t];
        tail_.store((t + 1) % N, std::memory_order_release);
        return true;
    }

private:
    std::array<T, N> buf_{};
    std::atomic<size_t> head_{0};
    std::atomic<size_t> tail_{0};
};

} // namespace signaling
