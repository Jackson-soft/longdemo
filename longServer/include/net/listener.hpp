#pragma once

#include "socket.hpp"
#include "utils/util.hpp"
#include <string>
#include <string_view>

namespace Uranus
{
namespace Net
{
// 监听器接口
class Listener
{
public:
    Listener() = default;
    virtual ~Listener() {}

    virtual int Listen(std::string_view network, std::string_view address) = 0;

    virtual int Accept() = 0;

    virtual void Close() = 0;

    virtual std::string Address() const = 0;
};

class TcpListener : public Listener
{
public:
    TcpListener() {}

    ~TcpListener() override {}

    int Listen(std::string_view network, std::string_view address) override
    {
        if (0 == network.size() || 0 == address.size()) {
            return 0;
        }
    }

    int Accept() override { return 0; }

    void Close() override { mSocket.Close(); }

    std::string Address() const override { return ""; }

private:
    Socket mSocket;
};
}  // namespace Net
}  // namespace Uranus