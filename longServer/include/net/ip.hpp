#pragma once

#include <array>
#include <netinet/in.h>
#include <string>
#include <string_view>

namespace Uranus
{
namespace Net
{
// IP interface
class IP
{
public:
    IP()          = default;
    virtual ~IP() = default;

    virtual bool operator==(const IP &lhs) = 0;

    virtual const std::string &String() const = 0;
};

// IPV6
class IPV6 : public IP
{
public:
    IPV6()          = default;
    virtual ~IPV6() = default;

    bool operator==(const IP &lhs) override {}

public:
    // 是否是 IPVe的 ::
    bool IsUnspecified()
    {
        std::array<std::uint8_t, 16> unspecified{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        return mIP == unspecified;
    }

    // 是否是回环地址
    bool IsLoopback()
    {
        std::array<std::uint8_t, 16> loopback{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
        return mIP == loopback;
    }

    std::string &String() override { return ""; }

private:
    void parse(std::string_view str)
    {
        if (str.empty()) {
            mIP.fill(0);
        } else if (str.size() >= 2 && str.at(0) == ':' && str.at(1) == ':') {
        }
    }

private:
    const uint mLen{16};
    // ipv6 长度是16  std::byte == std::uint8_t
    std::array<std::uint8_t, 16> mIP;
};
}  // namespace Net
}  // namespace Uranus
