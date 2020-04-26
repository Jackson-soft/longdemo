#pragma once

// TCP 客户端

#include "dialer.hpp"
#include "utils/util.hpp"
#include <cstdint>
#include <memory>
#include <string_view>

namespace Uranus
{
namespace Net
{
class TcpClient : public Utils::Noncopyable
{
public:
    TcpClient()  = default;
    ~TcpClient() = default;

    bool Dial(std::string_view ip, const std::uint16_t port)
    {
        if (ip.empty() || port <= 0)
            return false;
        return false;
    }

private:
    std::unique_ptr<Dialer> dialer;
};
}  // namespace Net
}  // namespace Uranus
