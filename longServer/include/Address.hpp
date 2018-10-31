#pragma once

#include <string>
#include <string_view>

//网络地址接口
class Address
{
public:
	Address() {}
	virtual ~Address() {}

	virtual Address(const Address &obj) = 0; // 复制构造函
	virtual Address(Address &&obj)		= 0; // 移动构造函数

	virtual Address &operator=(const Address &obj) = 0;	// copy
	virtual Address &operator=(Address &&obj)		  = 0; // move
	virtual bool operator==(const Address &obj) const = 0;
	virtual bool operator!=(const Address &obj) const = 0;

	//网络类型： tcp,udp,unix
	virtual std::string Network() = 0;

	//网络地址： 127.0.0.1:80,2000:0000:0000:0000:0001:2345:6789:abcd::5060
	virtual std::string String() = 0;
};

// ipv6地址类
class IPv6Addr : public Address
{
public:
	IPv6Addr() {}
	~IPv6Addr() override {}

	IPv6Addr(const IPv6Addr &obj) override {} // 复制构造函
	IPv6Addr(IPv6Addr &&obj) override {}	  // 移动构造函数

	IPv6Addr &operator=(const IPv6Addr &obj) override {} // copy
	IPv6Addr &operator=(IPv6Addr &&obj) override {}		 // move
	bool operator==(const IPv6Addr &obj) const override {}
	bool operator!=(const IPv6Addr &obj) const override {}
	// 解析地址
	bool ResolveAddr(std::string_view addr, std::string_view net = "tcp")
	{
		if (net != "tcp" || net != "udp") {
			return false;
		}
		mNetwork = net
	}
	 std::string Network() override { return mNetwork; }

	std::string String() override { return ""; }

private:
	std::string mNetwork{""};
	std::string mIP{""};
	unsigned short mPort{0};
};

// Unix地址类
class UnixAddr : public Address
{
public:
	UnixAddr() {}
	~UnixAddr() override {}

	UnixAddr(std::string_view net, std::string_view addr)
	{
		if ((net == "unix" || net == "unixgram" || net == "unixpacket") &&
			(!addr.empty())) {
			mNetwork = net;
			mAddress = addr;
		}
	}

	std::string Network() override { return mNetwork; }

	std::string String() override { return mAddress; }

private:
	std::string mNetwork{""};
	std::string mAddress{""};
};