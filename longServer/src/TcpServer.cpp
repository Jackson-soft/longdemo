#include "TcpServer.h"
#include <thread>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

TcpServer::TcpServer() {}

TcpServer::TcpServer(std::string ip, int port, unsigned int workNum)
	: bRunning(true), sIp(ip), nPort(port), nWorkers(workNum)
{
	if (nWorkers == 0) {
		nWorkers = std::thread::hardware_concurrency();
	}
}

TcpServer::~TcpServer() { Stop(); }

bool TcpServer::initServer()
{
	for (unsigned int i = 0; i < nWorkers; ++i) {
		auto pid = fork();
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

void TcpServer::workerRun()
{
	while (1) {
		/* code */
	}
}

//子进程循环
void TcpServer::Run()
{
	initServer();
	while (1) {
	}
}

void TcpServer::Stop() { bRunning = false; }
