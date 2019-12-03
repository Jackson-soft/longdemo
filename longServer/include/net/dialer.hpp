#pragma once

#include "address.hpp"
#include "socket.hpp"
#include "utils/util.hpp"
#include <chrono>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

// 连接器
namespace Uranus
{
namespace Net
{
// connection 接口
class Conn
{
public:
    Conn() = default;
    virtual ~Conn() {}

    virtual int Read() = 0;

    virtual int Write(std::byte b) = 0;

    virtual void Close() = 0;

    virtual Address LocalAddr() = 0;

    virtual Address RemoteAddr() = 0;
};

// 连接器类
class Dialer : public Conn, Uranus::Noncopyable, std::enable_shared_from_this<Dialer>
{
public:
    Dialer() = default;
    ~Dialer() override { Close(); }

    //连接到网络地址
    bool Dial(const std::string_view network, const std::string_view ip, const unsigned short port)
    {
        if (network.empty() || ip.empty() || port <= 0) {
            return false;
        }
        mRemoteAddr = std::move(ip);
        mPort       = port;

        if (!mSocket.NewSocket(network))
            return false;

        return mSocket.Connect(ip, port) == 0 ? true : false;
    }

    //本地网络地址
    std::string LocalAddr() const { return ""; }

    //远程网络地址
    std::string RemoteAddr() const { return ""; }

    int Read() { return 0; }

    int Write() { return 0; }

    bool SetKeepAlive(bool on) { return mSocket.SetKeeplive(on) == 0 ? true : false; }

    void Close() { mSocket.Close(); }

    int Shutdown() { return mSocket.ShutDown(); }

private:
    // Socket对象
    Socket mSocket;

    std::chrono::duration<int> mTimeout;

    //客户端地址
    std::string mLocalAddr{""};

    //远程地址
    std::string mRemoteAddr{""};

    unsigned short mPort{0};

    std::chrono::duration<int> mKeepAlive;
};
}  // namespace Net
}  // namespace Uranus