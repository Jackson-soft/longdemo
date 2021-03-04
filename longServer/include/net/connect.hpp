#pragma once

#include "address.hpp"
#include "socket.hpp"

namespace uranus::net
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

class TcpConn
{
public:
    //本地网络地址
    std::string LocalAddr() const { return ""; }

    //远程网络地址
    std::string RemoteAddr() const { return ""; }
};
}  // namespace uranus::net
