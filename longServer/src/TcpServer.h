#pragma once

#include <boost/noncopyable.hpp>
#include <string_view>

// Tcp 服务器
class TcpServer : boost::noncopyable
{
public:
	TcpServer();
    TcpServer(std::string_view ip,
              unsigned short port,
              unsigned int workNum = 0);
	~TcpServer();

	// 主循环
	void Run();

	void Stop();

private:
    // 初始化master,worker进程
	bool initServer();

    // worker
	void workerRun();

private:
    std::string_view mIP; // address
    unsigned short mPort;
    unsigned int mWorkers; // 工作进程数量
    bool mRunning;
};
