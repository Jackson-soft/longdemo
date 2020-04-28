#pragma once

// TCP 客户端
#include "dialer.hpp"
#include "utils/noncopyable.hpp"
#include <cstdint>
#include <memory>
#include <string_view>

namespace Uranus::Net
{
class TcpClient : public Utils::Noncopyable
{
public:
    TcpClient()  = default;
    ~TcpClient() = default;

    auto Dial(std::string_view ip, const std::uint16_t port) -> bool
    {
        if (ip.empty() || port <= 0)
            return false;
        return false;
    }

private:
    std::unique_ptr<Dialer> dialer;
};
}  // namespace Uranus::Net
