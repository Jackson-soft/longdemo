#pragma once

#include "fmt/core.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace uranus::net {
// 网络地址接口
class Address {
public:
    Address()          = default;
    virtual ~Address() = default;

    Address(const Address &obj)                     = default;  // copy
    Address(Address &&obj) noexcept                 = default;  // move
    auto operator=(const Address &obj) -> Address & = default;
    auto operator=(Address &&obj) -> Address      & = default;

    // 网络类型： tcp,udp,unix
    [[nodiscard]] virtual auto Network() const -> const std::string & = 0;

    // 网络地址： ipv4:127.0.0.1:80; ipv6:2000:0000:0000:0000:0001:2345:6789:abcd::5060
    [[nodiscard]] virtual auto String() const -> std::string = 0;
};

// ipv6地址类
class IPv6Addr : public Address {
public:
    IPv6Addr()           = default;
    ~IPv6Addr() override = default;

    // 复制构造函
    IPv6Addr(const IPv6Addr &obj) : network_(obj.network_), host_(obj.host_), port_(obj.port_), zone_(obj.zone_) {}

    // 移动构造函数
    IPv6Addr(IPv6Addr &&obj) noexcept
        : network_(std::move(obj.network_)), host_(obj.host_), port_(obj.port_), zone_(std::move(obj.zone_)) {}

    // copy
    auto operator=(const IPv6Addr &obj) -> IPv6Addr & {
        if (this != &obj) {
            network_ = obj.network_;
        }
        return *this;
    }

    // move
    auto operator=(IPv6Addr &&obj) noexcept -> IPv6Addr & {
        network_ = std::move(obj.network_);
        return *this;
    }

    // 解析地址
    auto Resolve(std::string_view addr, std::string_view net = "tcp") -> bool {
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
    std::string               network_;
    std::array<std::byte, 16> host_;
    std::uint16_t             port_{0};
    std::string               zone_;  // IPv6 scoped addressing zone
};

// Unix地址类
class UnixAddr : public Address {
public:
    UnixAddr()           = default;
    ~UnixAddr() override = default;

    UnixAddr(const UnixAddr &obj) : network_(obj.network_), address_(obj.address_) {}

    UnixAddr(UnixAddr &&obj) noexcept : network_(std::move(obj.network_)), address_(std::move(obj.address_)) {}

    auto operator=(const UnixAddr &obj) -> UnixAddr & {
        if (this != &obj) {
            network_ = obj.network_;
            address_ = obj.address_;
        }
        return *this;
    }

    auto operator=(UnixAddr &&obj) noexcept -> UnixAddr & {
        network_ = std::move(obj.network_);
        address_ = std::move(obj.address_);
        return *this;
    }

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
