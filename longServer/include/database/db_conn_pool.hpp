#pragma once

#include "db_conn.hpp"
#include "dsn.hpp"
#include "utils/noncopyable.hpp"
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <string_view>
#include <utility>

namespace Uranus::Database
{
class DBConnPool : public Utils::Noncopyable
{
public:
    DBConnPool() = default;
    ~DBConnPool() { Destroy(); }

    static std::shared_ptr<DBConnPool> Get()
    {
        static std::shared_ptr<DBConnPool> instance = std::make_shared<DBConnPool>();
        return instance;
    }

public:
    // 初始化
    bool Initialize(const std::string_view host,
                    const std::string_view user,
                    const std::string_view pwd,
                    const std::string_view db,
                    const std::uint32_t maxIdle = 1,
                    const std::uint32_t maxConn = 1,
                    const std::uint32_t port    = 3306)
    {
        std::lock_guard<std::mutex> lock(mLock);
        if (host.size() * user.size() * pwd.size() == 0)
            return false;
        mHost   = host;
        mUser   = user;
        mPasswd = pwd;
        mDBName = db;
        mPort   = port;

        return initDeque(maxIdle, maxConn);
    }

    // dsn
    bool Initialize(const std::string_view connStr, const std::uint32_t maxIdle = 1, const std::uint32_t maxConn = 1)
    {
        std::lock_guard<std::mutex> lock(mLock);
        if (connStr.empty())
            return false;
        Uranus::DSN dsn;
        if (!dsn.Parse(connStr))
            return false;

        mHost   = dsn.Host();
        mUser   = dsn.User();
        mPasswd = dsn.Passwd();
        mDBName = dsn.DBName();
        mPort   = dsn.Port();

        return initDeque(maxIdle, maxConn);
    }

    // 获取一个连接
    std::shared_ptr<DBConn> GetConn()
    {
        std::lock_guard<std::mutex> lock(mLock);
        std::shared_ptr<DBConn> temp;
        if (!mFreeConn.empty()) {
            temp = mFreeConn.front();
            mFreeConn.pop_front();
        } else if (mNumOpen < mMaxOpen) {
            auto p = std::make_shared<DBConn>();
            if (p->Connect(mHost, mUser, mPasswd, mDBName, mPort))
                temp = p;
        } else {
            return nullptr;
        }
        mNumOpen++;
        return temp;
    }

    // 归还一个连接
    void PutConn(std::shared_ptr<DBConn> conn)
    {
        std::lock_guard<std::mutex> lock(mLock);
        mFreeConn.emplace_back(conn);
    }

    // 设置最大空闲连接数
    void SetMaxIdleConns(const std::uint32_t n = 0)
    {
        std::lock_guard<std::mutex> lock(mLock);
        mMaxIdle = n >= mMaxOpen ? mMaxOpen : n;
    }

    // 设置最大连接数
    void SetMaxOpenConns(const std::uint32_t n = 1)
    {
        std::lock_guard<std::mutex> lock(mLock);
        mMaxOpen = n <= mMaxIdle ? mMaxIdle : n;
    }

    // 设置连接的生命周期
    void SetConnMaxLifetime(const std::uint32_t d = 0) { std::lock_guard<std::mutex> lock(mLock); }

    // 销毁
    void Destroy()
    {
        std::lock_guard<std::mutex> lock(mLock);
        for (auto &it : mFreeConn) {
            it->Close();
        }
        mFreeConn.clear();
        mNumOpen = 0;
    }

private:
    bool initDeque(const std::uint32_t maxIdle, const std::uint32_t maxConn)
    {
        mMaxIdle = maxIdle > 0 ? maxIdle : 1;
        mMaxOpen = maxConn > mMaxIdle ? maxConn : mMaxIdle;
        for (std::uint32_t i = 0; i < mMaxIdle; ++i) {
            auto p = std::make_shared<DBConn>();
            if (p->Connect(mHost, mUser, mPasswd, mDBName, mPort)) {
                mFreeConn.emplace_back(std::move(p));
                mNumOpen++;
            }
        }
        return !mFreeConn.empty();
    }

private:
    std::mutex mLock;
    std::string mHost;
    std::string mUser;
    std::string mPasswd;
    std::string mDBName;
    std::uint32_t mPort{3306};
    std::uint32_t mMaxIdle{1};  // 最大空闲数
    std::uint32_t mMaxOpen{1};  // 最大连接数

    std::uint32_t mNumOpen{0};  // 当前连接数
    std::deque<std::shared_ptr<DBConn>> mFreeConn;
};
}  // namespace Uranus::Database
