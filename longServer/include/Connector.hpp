#pragma once

#include "Util.hpp"
#include <memory>
#include <unistd.h>

// 连接器类
class Connector : public Noncopyable, std::enable_shared_from_this<Connector>
{
public:
	Connector() {}
	~Connector() {}

	void Close() { ::close(mSocket); }

	int Read() { return 0; }

	int Write() { return 0; }

	bool SetKeepAlive(bool keepalive) { return true; }

private:
	// socket描述符
	int mSocket;
};
