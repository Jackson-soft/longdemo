#pragma once

#include "Socket.hpp"
#include "Util.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

// 连接器类
class Connector : public Noncopyable, std::enable_shared_from_this<Connector>
{
public:
	Connector() = default;
	~Connector() { Close(); }

	//连接到网络地址
	bool
	Dial(std::string_view network, std::string_view ip, unsigned short port)
	{
		if (network.empty() || ip.empty() || port <= 0) {
			return false;
		}
		mNetAddr = std::move(ip);
		mPort	= port;

		if (!mSocket.NewSocket(network))
			return false;

		return mSocket.Connect(port, ip) == 0 ? true : false;
	}

	//本地网络地址
	std::string LocalAddr() { return ""; }

	//远程网络地址
	std::string RemoteAddr() { return ""; }

	int Read() { return 0; }

	int Write() { return 0; }

	bool SetKeepAlive(bool on)
	{
		return mSocket.SetKeeplive(on) == 0 ? true : false;
	}

	void Close() { mSocket.Close(); }

	int Shutdown() { return mSocket.ShutDown(); }

private:
	// Socket对象
	Socket mSocket;

	std::chrono::duration<int> mTimeout;

	//客户端ip
	std::string mLocalAddr{""};

	//网络ip
	std::string mNetAddr{""};

	unsigned short mPort{0};

	std::chrono::duration<int> mKeepAlive;
};
