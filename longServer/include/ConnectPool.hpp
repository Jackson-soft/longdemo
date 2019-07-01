#pragma once
// 连接池

#include "Util.hpp"
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <string_view>

template<typename T>
class ConnectPool : public Noncopyable
{
public:
    static ConnectPool<T> *Get()
    {
        static ConnectPool<T> connPool;
        return &connPool;
    }

    void Initalize(std::string_view conn, int maxConn = 10, int maxIdle = 5)
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
        if (conn == nullptr) {
            return;
        }

        std::unique_lock<std::mutex> lock(mMutex);
        mPool.push_back(conn);
        lock.unlock();
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
    std::string_view mConnect;  // 连接
    int mMaxIdle{0};            // 最大空闲数
    int mMaxConn{10};           // 最大连接数
    int mNumOpen{0};            //打开的连接数
    std::condition_variable mCondition;
    std::once_flag mFlag;
    std::deque<std::shared_ptr<T>> mPool;
};
