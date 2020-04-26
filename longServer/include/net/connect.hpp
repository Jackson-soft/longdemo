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

namespace Uranus
{
namespace Net
{
// connection 接口
class Conn
{
public:
    Conn()          = default;
    virtual ~Conn() = default;

    virtual int Read() = 0;

    virtual int Write(std::vector<std::byte> &data) = 0;

    virtual void Close() = 0;

    virtual void Shutdown() = 0;

    virtual const std::string RemoteAddr() const = 0;
};

class TcpConn : public Conn, std::enable_shared_from_this<Conn>
{
public:
    TcpConn() = default;
    ~TcpConn() override { Close(); }

    TcpConn(const int fd, std::string_view net) : remoteFD(std::make_unique<Socket>(fd, net)) {}

    int Read() override { return 0; }

    int Write(std::vector<std::byte> &data) override { return 0; }

    void Close() override { remoteFD->Close(); }

    void Shutdown() override { remoteFD->Shutdown(); }

    //远程网络地址
    const std::string RemoteAddr() const override { return ""; }

private:
    std::unique_ptr<Socket> remoteFD;
};
}  // namespace Net
}  // namespace Uranus
