#pragma once

#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class Socket
{
public:
    Socket();
    ~Socket();

    bool Listen(std::string_view ip, unsigned short port);

    // 返回接收的socket文件描述符
    int Accept();

    //优雅关闭
    int ShutDown();

    int Close();

private:
    int mSocket; //
};
