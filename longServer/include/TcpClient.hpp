#pragma once

// TCP 客户端

#include "Connector.hpp"
#include "Util.hpp"
#include <string_view>

class TcpClient : public Noncopyable
{
public:
	TcpClient() {}
	~TcpClient() {}

	bool Dial(std::string_view ip, unsigned short port) {}

private:
	Connector mTcpConn;
};