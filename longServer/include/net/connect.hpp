#pragma once

#include "address.hpp"
#include "socket.hpp"

#include <cstddef>

namespace uranus::net {
// connection 接口
class Conn
{
public:
    Conn()                                 = default;
    virtual ~Conn()                        = default;

    virtual auto Read() -> int             = 0;
    virtual auto Write(std::byte b) -> int = 0;
    virtual void Close()                   = 0;
    virtual auto LocalAddr() -> Address    = 0;
    virtual auto RemoteAddr() -> Address   = 0;
};

class TcpConn
{
public:
    //本地网络地址
    auto LocalAddr() const -> std::string {
        return "";
    }

    //远程网络地址
    auto RemoteAddr() const -> std::string {
        return "";
    }
};
}  // namespace uranus::net
