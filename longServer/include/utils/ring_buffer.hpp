#pragma once

#include "noncopyable.hpp"
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace Uranus::Utils
{
// 环形缓存
template<typename T>
class RingBuffer : public Noncopyable
{
public:
    RingBuffer() : RingBuffer(16) {}

    explicit RingBuffer(const int size) : maxSize(size), writeIndex(0), readIndex(0) { data.reserve(maxSize); }

    ~RingBuffer() { Clear(); }

    //读取
    auto Read()
    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        int nTmp = readIndex;
        readIndex++;
        readIndex = readIndex >= maxSize ? 0 : readIndex;
        return data[nTmp];
    }

    // 写入
    void Write(const T &value)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        data[writeIndex] = value;
        writeIndex++;
        writeIndex = writeIndex >= maxSize ? 0 : writeIndex;
    }

    // 清除
    void Clear() noexcept
    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        data.clear();
    }

private:
    int maxSize;
    int writeIndex;
    int readIndex;
    mutable std::shared_mutex mutex;
    std::vector<T> data;
};
}  // namespace Uranus::Utils