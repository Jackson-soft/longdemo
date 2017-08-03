#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Net
{
public:
	Net();
	~Net();

	bool Listen(const char *ip, unsigned short port);

	int Accept();

	//优雅关闭
	int ShutDown();

	int Close();

private:
	int fSocket; //
};
