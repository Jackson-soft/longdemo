#include <iostream>

#include "../TcpServer.h"

int main()
{
	TcpServer server("127.0.0.1", 8080);
	server.Run();
	return 0;
}