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

    auto Accept() -> int override {
        return 0;
    }

    void Close() override {
        socket_.Close();
    }

    auto Address() const -> const std::string & override {
        return "";
    }

private:
    net::Socket socket_;
};
}  // namespace uranus::net
