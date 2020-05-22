#pragma once

#include <cstdint>
#include <fmt/core.h>
#include <netinet/in.h>
#include <string>
#include <string_view>

namespace Uranus::Net
{
//网络地址接口
class Address
{
public:
    Address()          = default;
    virtual ~Address() = default;

    [[nodiscard]] virtual auto Network() const -> std::string = 0;

    //网络地址： ipv4:127.0.0.1:80; ipv6:2000:0000:0000:0000:0001:2345:6789:abcd::5060
    [[nodiscard]] virtual auto String() const -> std::string = 0;
};

class IPv4Addr : public Address
{
public:
    IPv4Addr()           = default;
    ~IPv4Addr() override = default;

    [[nodiscard]] auto Network() const -> std::string override { return "ipv4"; }

    [[nodiscard]] auto String() const -> std::string override { return fmt::format("{}:{}", host, port); }

private:
    std::string host;
    std::uint16_t port;
};

// "[host]:port" or "[host%zone]:port"
// ipv6地址类
class IPv6Addr : public Address
{
public:
    IPv6Addr()           = default;
    ~IPv6Addr() override = default;

    IPv6Addr(const IPv6Addr &obj) = default;  // 复制构造函
    IPv6Addr(IPv6Addr &&obj)      = default;  // 移动构造函数

    IPv6Addr &operator=(const IPv6Addr &obj) = default;  // copy
    IPv6Addr &operator=(IPv6Addr &&obj) = default;       // move

    auto Resolve(std::string_view ip, const std::uint16_t in) -> bool
    {
        if (ip.empty() || in <= 0)
            return false;
        host = ip;
        port = in;
        return true;
    }

    // 解析地址
    auto Resolve(std::string_view addr) -> bool
    {
        if (addr.empty())
            return false;
        if (addr.at(0) != '[')
            return false;

        auto pos      = addr.find(']');
        auto hostZone = addr.substr(1, pos - 1);
        auto tempPort = addr.substr(pos + 2);
        auto zonePos  = hostZone.find('%');
        if (zonePos == std::string::npos) {
            // not exit
            host = hostZone;
        } else {
            host = hostZone.substr(0, zonePos);
            zone = hostZone.substr(zonePos + 1);
        }
        std::string temp(tempPort);
        port = std::stoi(temp);
        return true;
    }

    [[nodiscard]] auto Network() const -> std::string override { return "ipv6"; }

    [[nodiscard]] auto String() const -> std::string override
    {
        if (!zone.empty()) {
            return fmt::format("[{}%{}]:{}", host, zone, port);
        }
        return fmt::format("[{}]:{}", host, port);
    }

    [[nodiscard]] auto Host() const -> const std::string & { return host; }

    [[nodiscard]] auto Post() const -> const std::uint16_t { return port; }

    [[nodiscard]] auto Zone() const -> const std::string & { return zone; }

    auto isLoop() -> bool { return host == "::1"; }

private:
    std::string host{""};
    std::string zone{""};
    std::uint16_t port{0};
};

// Unix地址类
class UnixAddr : public Address
{
public:
    UnixAddr()           = default;
    ~UnixAddr() override = default;

    [[nodiscard]] auto Network() const -> std::string override { return "unix"; }

    [[nodiscard]] auto String() const -> std::string override { return address; }

private:
    std::string address{""};
};
}  // namespace Uranus::Net
