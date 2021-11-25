#pragma once

#include <array>
#include <netinet/in.h>
#include <string>
#include <string_view>

namespace uranus::net {
// IP interface
class IP
{
public:
    IP()                                           = default;
    virtual ~IP()                                  = default;

    virtual auto operator==(const IP &lhs) -> bool = 0;
    virtual auto String() -> std::string         & = 0;
};

// IPV6
class IPV6 : public IP
{
public:
    IPV6()           = default;
    ~IPV6() override = default;

    auto operator==(const IP &lhs) -> bool override {}

    // 是否是 IPV6的 ::
    auto IsUnspecified() -> bool {
        const std::array<std::uint8_t, 16> unspecified{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        return ip_ == unspecified;
    }

    // 是否是回环地址
    auto IsLoopback() -> bool {
        const std::array<std::uint8_t, 16> loopback{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
        return ip_ == loopback;
    }

    auto String() -> std::string & override {
        return "";
    }

private:
    void parse(std::string_view str) {
        if (str.empty()) {
            ip_.fill(0);
        } else if (str.size() >= 2 && str.at(0) == ':' && str.at(1) == ':') {
        }
    }

    const uint                   len_{16};
    // ipv6 长度是16  std::byte == std::uint8_t
    std::array<std::uint8_t, 16> ip_;
};
}  // namespace uranus::net
