#pragma once

#include "utils/util.hpp"
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace Uranus
{
// 环形缓存
template<typename T>
class RingBuffer : public Utils::Noncopyable
{
public:
    RingBuffer() : RingBuffer(16) {}

    explicit RingBuffer(int size) : mMaxSize(size) { mData.reserve(mMaxSize); }

    ~RingBuffer() { Clear(); }

    //读取
    T Read()
    {
        std::shared_lock<std::shared_mutex> lock(mMutex);

        int nTmp = mReadIndex;
        mReadIndex++;
        mReadIndex = mReadIndex >= mMaxSize ? 0 : mReadIndex;
        return mData[nTmp];
    }

    // 写入
    void Write(const T &value)
    {
        std::unique_lock<std::shared_mutex> lock(mMutex);

        mData[mWriteIndex] = value;
        mWriteIndex++;
        mWriteIndex = mWriteIndex >= mMaxSize ? 0 : mWriteIndex;
    }

    // 清除
    void Clear() noexcept
    {
        std::unique_lock<std::shared_mutex> lock(mMutex);
        mData.clear();
    }

private:
    mutable std::shared_mutex mMutex;
    std::vector<T> mData;
    int mMaxSize;
    int mWriteIndex{0};
    int mReadIndex{0};
};
}  // namespace Uranus