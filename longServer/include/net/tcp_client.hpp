#pragma once

// TCP 客户端

#include "dialer.hpp"
#include "utils/util.hpp"
#include <cstdint>
#include <string_view>

namespace Uranus::Net
{
class TcpClient: public Utils::Noncopyable
{
public:
    TcpClient()  = default;
    ~TcpClient() = default;

    bool Dial(std::string_view ip, std::uint32_t port) { return false; }

private:
    Net::Dialer dialer;
};
}  // namespace Uranus::Net