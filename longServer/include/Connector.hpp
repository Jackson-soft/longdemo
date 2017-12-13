#pragma once

#include "Util.hpp"

// 连接器类
class Connector : public Noncopyable
{
public:
	Connector();
	~Connector();

	void Close();

	int Read();

	int Write();

	bool SetKeepAlive(bool keepalive);

private:
	// socket描述符
	int mSocket;
};

Connector::Connector() {}

Connector::~Connector() {}