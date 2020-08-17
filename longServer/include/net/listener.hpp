#pragma once

#include "socket.hpp"
#include "utils/util.hpp"
#include <string>
#include <string_view>

namespace Uranus::Net
{
// 监听器接口
class Listener
{
public:
    Listener()          = default;
    virtual ~Listener() = default;

    virtual int Accept() = 0;

    virtual void Close() = 0;

    virtual const std::string &Address() const = 0;
};

class TcpListener: public Listener
{
public:
    TcpListener() = default;

    ~TcpListener() override = default;

    int Accept() override { return 0; }

    void Close() override { mSocket.Close(); }

    const std::string &Address() const override { return ""; }

private:
    Socket mSocket;
};
}  // namespace Uranus::Net
