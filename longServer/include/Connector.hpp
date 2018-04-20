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
	Connector() {}
	~Connector() { Close(); }

	bool
	Dial(std::string_view network, std::string_view ip, unsigned short port)
	{
		mLocalAddr = std::move(ip);
		mPort	  = port;

		if (!mSocket.NewSocket(network))
			return false;

		return mSocket.Connect(ip, port) == 0 ? true : false;
	}

	void Close() { mSocket.Close(); }

	int Read() { return 0; }

	int Write() { return 0; }

	bool SetKeepAlive(bool on)
	{
		return mSocket.SetKeeplive(on) == 0 ? true : false;
	}

private:
	// Socket对象
	Socket mSocket;

	std::chrono::duration<int> mTimeout;

	//客户端ip
	std::string mLocalAddr{""};

	unsigned short mPort{0};

	std::chrono::duration<int> mKeepAlive;
};
