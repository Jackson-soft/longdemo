#pragma once

#include <string>

// Tcp 服务器
class TcpServer
{
public:
	TcpServer();
	TcpServer(std::string ip, int port, unsigned int workNum = 0);
	~TcpServer();

	// 主循环
	void Run();

	void Stop();

private:
	//初始化master,worker进程
	bool initServer();

private:
	bool bRunning;
	std::string sIp;
	int nPort;
	unsigned int nWorkers; //工作进程数量
};
