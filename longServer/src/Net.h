#pragma once

#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Socket
{
public:
	Socket();
	Socket(const int fd);
	~Socket();

	//监听服务器
	bool Listen(unsigned short port, std::string_view ip = {""});

	//连接目标服务器
	bool Dial(unsigned short port, std::string_view ip);

	// 返回接收的socket文件描述符
	int Accept();

	int SetKeeplive(bool on);

	//优雅关闭
	int ShutDown();

	//关闭
	int Close();

private:
	int mSocket{0}; //

	//
	bool createSocket();

	//
	bool bindSocket(unsigned short port, std::string_view ip = {""});
};
