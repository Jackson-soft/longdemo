#pragma once

#include "fmt/core.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace uranus::net {
//网络地址接口
class Address {
public:
    Address()          = default;
    virtual ~Address() = default;

    virtual auto               operator=(const Address &obj) -> Address &= 0;      // copy
    virtual auto               operator=(Address &&obj) noexcept -> Address &= 0;  // move
    virtual auto               operator==(const Address &obj) const -> bool = 0;
    virtual auto               operator!=(const Address &obj) const -> bool = 0;

    //网络类型： tcp,udp,unix
    [[nodiscard]] virtual auto Network() const -> const std::string       & = 0;

    //网络地址： ipv4:127.0.0.1:80; ipv6:2000:0000:0000:0000:0001:2345:6789:abcd::5060
    [[nodiscard]] virtual auto String() const -> std::string                = 0;
};

// ipv6地址类
class IPv6Addr : public Address {
public:
    IPv6Addr()           = default;
    ~IPv6Addr() override = default;

    IPv6Addr(const IPv6Addr &obj) {}      // 复制构造函
    IPv6Addr(IPv6Addr &&obj) noexcept {}  // 移动构造函数

    auto operator=(const IPv6Addr &obj) -> IPv6Addr & {}      // copy
    auto operator=(IPv6Addr &&obj) noexcept -> IPv6Addr & {}  // move
    auto operator==(const IPv6Addr &obj) const -> bool {
        return false;
    }
    auto operator!=(const IPv6Addr &obj) const -> bool {
        return false;
    }

    // 解析地址
    auto ResolveAddr(std::string_view addr, std::string_view net = "tcp") -> bool {
        if (net != "tcp" || net != "udp") {
            return false;
        }
        if (addr.empty()) {
            return false;
        }
        network_ = net;
        return true;
    }

    auto IsLoopback() -> bool {
        return false;
    }

    [[nodiscard]] auto Network() const -> const std::string & override {
        return network_;
    }

    [[nodiscard]] auto String() const -> std::string override {
        return fmt::format("{}:{}", host_, port_);
    }

private:
    std::string   network_;
    std::string   host_;
    std::uint16_t port_{0};
};

// Unix地址类
class UnixAddr : public Address {
public:
    UnixAddr()           = default;
    ~UnixAddr() override = default;

    explicit UnixAddr(std::string_view addr, std::string_view net = "unix") {
        if ((net == "unix" || net == "unixgram" || net == "unixpacket") && (!addr.empty())) {
            network_ = net;
            address_ = addr;
        }
    }

    [[nodiscard]] auto Network() const -> const std::string & override {
        return network_;
    }

    [[nodiscard]] auto String() const -> std::string override {
        return address_;
    }

private:
    std::string network_;
    std::string address_;
};
}  // namespace uranus::net
