#pragma once

#include "Util.hpp"

// 接收器类
class Acceptor : public Noncopyable
{
public:
	Acceptor() {}
	~Acceptor() {}

	int Accept() { return 0; }

	void Close() {}

private:
	int mSocket;
};