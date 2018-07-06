#pragma once

#include "Socket.hpp"
#include "Util.hpp"
#include <string>

// 监听器类
class Listener : public Noncopyable
{
public:
	Listener() {}
	~Listener() {}

	int Listen() { return 0; }

	void Close() {}

	std::string Address() const { return ""; }

private:
	Socket mSocket;
};