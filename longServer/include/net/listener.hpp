#pragma once

#include "socket.hpp"

#include <string>
#include <string_view>

namespace uranus::net {
// 监听器接口
class Listener {
public:
    Listener()                                                        = default;
    virtual ~Listener()                                               = default;

    virtual auto               Accept() -> int                        = 0;

    virtual void               Close()                                = 0;

    [[nodiscard]] virtual auto Address() const -> const std::string & = 0;
};

class TcpListener : public Listener {
public:
    TcpListener()           = default;

    ~TcpListener() override = default;

    int Accept() override
    {
        return 0;
    }

    void Close() override
    {
        mSocket.Close();
    }

    const std::string &Address() const override
    {
        return "";
    }

private:
    Socket mSocket;
};
}  // namespace uranus::net
