#pragma once

// TCP 客户端

#include "dialer.hpp"
#include "utils/noncopyable.hpp"

#include <cstdint>
#include <string_view>

namespace uranus::net {
class TcpClient : public utils::Noncopyable {
public:
    TcpClient()  = default;
    ~TcpClient() = default;

    auto Dial(std::string_view ip, std::uint32_t port) -> bool {
        return false;
    }

private:
    net::Dialer dialer_;
};
}  // namespace uranus::net