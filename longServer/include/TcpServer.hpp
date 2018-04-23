#pragma once

#include "Acceptor.hpp"
#include "Util.hpp"
#include <atomic>
#include <string_view>

// Tcp 服务器
class TcpServer : public Noncopyable
{
public:
	TcpServer() {}

	~TcpServer() {}

	// 主循环
	bool ListenTcp(unsigned short port,
				   std::string_view ip  = {""},
				   unsigned int workNum = 0)
		: mPort(port), mIP(ip), mWorkers(workNum), mRunning(true)
	{
		if (mWorkers == 0) {
			mWorkers = std::thread::hardware_concurrency();
		}
		initServer();
		while (1) {
		}
	}

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
	unsigned short mPort;  // 端口
	std::string_view mIP;  // ip地址
	unsigned int mWorkers; // 工作进程数量

	std::atomic_bool mRunning{true};
	Acceptor mListener;
};
