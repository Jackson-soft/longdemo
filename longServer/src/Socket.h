#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


class Socket
{
private:
    int fSocket; //

public:
    Socket();
    ~Socket();
};
