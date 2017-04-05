#pragma once
#include <sys/socket.h>
#include <unistd.h>


class Socket
{
private:
    int fSocket; //

public:
    Socket();
    ~Socket();
};
