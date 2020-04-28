#pragma once

#include "connect.hpp"
#include "socket.hpp"
#include "utils/noncopyable.hpp"
#include <cstdint>
#include <memory>
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

    virtual auto Accept() -> std::shared_ptr<Conn> = 0;

    virtual void Close() = 0;

    virtual void Shutdown() = 0;
};

class TcpListener : public Listener
{
public:
    TcpListener() = default;

    ~TcpListener() override { listenFD->Close(); }

    auto Listen(std::string_view addr) -> bool
    {
        if (addr.empty())
            return false;

        listenFD = std::make_unique<Socket>();
        if (!listenFD->NewSocket("tcp"))
            return false;

        if (listenFD->Bind(8080))
            return false;

        return listenFD->Listen();
    }

    auto Listen(const std::uint16_t port, std::string_view ip = {""}) -> bool {}

    auto Accept() -> std::shared_ptr<Conn> override
    {
        auto clientFD = listenFD->Accept();

        return std::make_shared<TcpConn>(clientFD, "tcp");
    }

    void Close() override { listenFD->Close(); }

    void Shutdown() override { listenFD->Shutdown(); }

private:
    std::shared_ptr<Socket> listenFD;
};
}  // namespace Uranus::Net
