#pragma once

#include <string>
#include <string_view>

//网络地址接口
class Address
{
public:
	Address() {}
	virtual ~Address() {}

	//网络类型： tcp,udp,unix
	virtual std::string Network() = 0;

	//网络地址： 127.0.0.1:80,[2000:0000:0000:0000:0001:2345:6789:abcd]:5060
	virtual std::string String() = 0;
};

// ipv6地址类
class AddressV6 : public Address
{
public:
	AddressV6() {}
	~AddressV6() override {}

	std::string Network() override { return mNetwork; }

	std::string String() override {}

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