#pragma once

#include "address.hpp"
#include "socket.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

namespace Uranus::Net
{
// connection 接口
class Conn
{
public:
    Conn()          = default;
    virtual ~Conn() = default;

    virtual auto Read() -> int = 0;

    virtual auto Write(std::vector<std::byte> &data) -> int = 0;

    virtual void Close() = 0;

    virtual void Shutdown() = 0;

    [[nodiscard]] virtual auto RemoteAddr() const -> const std::string = 0;
};

class TcpConn : public Conn, std::enable_shared_from_this<Conn>
{
public:
    TcpConn() = default;
    ~TcpConn() override { Close(); }

    TcpConn(const int fd) : remoteFD(std::make_unique<Socket>(fd, "tcp")) {}

    auto Read() -> int override { return 0; }

    auto Write(std::vector<std::byte> &data) -> int override
    {
        if (data.empty())
            return 0;
        return 0;
    }

    void Close() override { remoteFD->Close(); }

    void Shutdown() override { remoteFD->Shutdown(); }

    //远程网络地址
    [[nodiscard]] auto RemoteAddr() const -> const std::string override { return ""; }

private:
    std::unique_ptr<Socket> remoteFD;
};
}  // namespace Uranus::Net
