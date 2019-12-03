#pragma once

// TCP 客户端

#include "dialer.hpp"
#include "utils/util.hpp"
#include <cstdint>
#include <string_view>

namespace Uranus
{
class TcpClient : public Noncopyable
{
public:
    TcpClient() {}
    ~TcpClient() {}

    bool Dial(std::string_view ip, std::uint32_t port) { return false; }

private:
    Dialer mDialer;
};
}  // namespace Uranus