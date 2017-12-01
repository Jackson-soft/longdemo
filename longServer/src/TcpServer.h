#pragma once

#include <boost/noncopyable.hpp>
#include <string_view>

// Tcp 服务器
class TcpServer : boost::noncopyable
{
public:
	TcpServer();
    TcpServer(unsigned short port,
              std::string_view ip  = {""},
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
    unsigned short mPort;
    std::string_view mIP;  // address
    unsigned int mWorkers; // 工作进程数量
    bool mRunning;
};
