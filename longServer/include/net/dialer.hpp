#pragma once

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
// 连接器类
class Dialer : public Utils::Noncopyable
{
public:
    Dialer() = default;
    ~Dialer() { Close(); }

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