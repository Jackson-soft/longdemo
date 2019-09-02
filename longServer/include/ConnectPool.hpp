#pragma once
// 连接池

#include "Util.hpp"
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <string_view>

namespace Uranus
{
template<typename T>
class ConnectPool : public Uranus::Noncopyable
{
public:
    static ConnectPool<T> *Get()
    {
        static ConnectPool<T> connPool;
        return &connPool;
    }

    void Initalize(const std::string_view conn, const int maxConn = 10, const int maxIdle = 5)
    {
        mConnect = conn;
        mMaxIdle = maxIdle;
        mMaxConn = maxConn;
    }

    std::shared_ptr<T> Get()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        while (mPool.empty()) {
            if (mCondition.wait_for(lock, std::chrono::seconds(3)) == std::cv_status::timeout) {
                return nullptr;
            }
        }

        int numFree = mPool.size();

        auto conn = mPool.front();
        mPool.pop_front();
    }

    void Put(std::shared_ptr<T> conn)
    {
        if (!conn) {
            return;
        }

        std::unique_lock<std::mutex> lock(mMutex);
        mPool.push_back(conn);
    }

private:
    ConnectPool()  = default;
    ~ConnectPool() = default;

    ConnectPool(const ConnectPool &) = delete;
    ConnectPool &operator=(const ConnectPool &) = delete;

    auto creatConn()
    {
        auto conn = std::make_shared<T>();
        return conn->connect(mConnect);
    }

private:
    std::mutex mMutex;
    std::string mConnect;  // 连接
    int mMaxIdle{0};       // 最大空闲数
    int mMaxConn{10};      // 最大连接数
    int mNumOpen{0};       //打开的连接数
    std::condition_variable mCondition;
    std::once_flag mFlag;
    std::deque<std::shared_ptr<T>> mPool;
};
}  // namespace Uranus
