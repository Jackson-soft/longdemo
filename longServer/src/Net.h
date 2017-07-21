#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


class Net
{
public:
    Net();
    ~Net();

    bool InitNet();

    int Accept();

    //优雅关闭
    int ShutDown();

    int Close();

private:
    int fSocket; //
};
