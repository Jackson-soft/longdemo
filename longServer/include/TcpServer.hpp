#pragma once

#include <boost/noncopyable.hpp>
#include <string_view>

// Tcp 服务器
class TcpServer : boost::noncopyable
{
public:
	//委托构造函数
	TcpServer() : TcpServer(8088, "127.0.0.1") {}

	TcpServer(unsigned short port,
			  std::string_view ip  = {""},
			  unsigned int workNum = 0)
		: mPort(port), mIP(ip), mWorkers(workNum), mRunning(true)
	{
		if (mWorkers == 0) {
			mWorkers = std::thread::hardware_concurrency();
		}
	}

	~TcpServer() { Stop(); }

	// 主循环
	void Run()
	{
		initServer();
		while (1) {
		}
	}

	void Stop() { mRunning = false; }

private:
	// 初始化master,worker进程
	bool initServer()
	{
		for (unsigned int i = 0; i < mWorkers; ++i) {
			auto pid = ::fork();
			switch (pid) {
			case -1:
				//错误处理
				break;
			case 0:
				//子进程
				workerRun();
				break;
			default:
				//父进程
				break;
			}
		}
		return true;
	}

	// worker
	void workerRun()
	{
		while (1) {
			/* code */
		}
	}

private:
	unsigned short mPort;
	std::string_view mIP;  // address
	unsigned int mWorkers; // 工作进程数量
	bool mRunning;
};
