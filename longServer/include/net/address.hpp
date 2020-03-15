#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace Uranus
{
namespace Net
{
//网络地址接口
class Address
{
public:
    Address()          = default;
    virtual ~Address() = default;

    virtual Address &operator=(const Address &obj) = 0;     // copy
    virtual Address &operator=(Address &&obj)         = 0;  // move
    virtual bool operator==(const Address &obj) const = 0;
    virtual bool operator!=(const Address &obj) const = 0;

    //网络类型： tcp,udp,unix
    [[nodiscard]] virtual const std::string &Network() const = 0;

    //网络地址： ipv4:127.0.0.1:80; ipv6:2000:0000:0000:0000:0001:2345:6789:abcd::5060
    [[nodiscard]] virtual const std::string &String() const = 0;
};

// ipv6地址类
class IPv6Addr : public Address
{
public:
    IPv6Addr()           = default;
    ~IPv6Addr() override = default;

    IPv6Addr(const IPv6Addr &obj) {}  // 复制构造函
    IPv6Addr(IPv6Addr &&obj) {}       // 移动构造函数

    IPv6Addr &operator=(const IPv6Addr &obj) {}  // copy
    IPv6Addr &operator=(IPv6Addr &&obj) {}       // move
    bool operator==(const IPv6Addr &obj) const { return false; }
    bool operator!=(const IPv6Addr &obj) const { return false; }

    // 解析地址
    bool ResolveAddr(std::string_view addr, std::string_view net = "tcp")
    {
        if (net != "tcp" || net != "udp") {
            return false;
        }
        mNetwork = net;
        return true;
    }
    const std::string &Network() const override { return mNetwork; }

    const std::string &String() const override { return ""; }

private:
    std::string mNetwork{""};
    std::string mIP{""};
    std::uint16_t mPort{0};
};

// Unix地址类
class UnixAddr : public Address
{
public:
    UnixAddr() {}
    ~UnixAddr() override {}

    UnixAddr(const std::string_view net, const std::string_view addr)
    {
        if ((net == "unix" || net == "unixgram" || net == "unixpacket") && (!addr.empty())) {
            mNetwork = net;
            mAddress = addr;
        }
    }

    std::string &Network() override { return mNetwork; }

    std::string &String() override { return mAddress; }

private:
    std::string mNetwork{""};
    std::string mAddress{""};
};
}  // namespace Net
}  // namespace Uranus
