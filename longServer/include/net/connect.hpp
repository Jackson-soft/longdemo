#pragma once

#include <cstddef>
#include <vector>

namespace uranus::net {
// connection 接口
class Conn {
public:
    Conn()          = default;
    virtual ~Conn() = default;

    virtual auto Read(std::vector<std::byte> data) -> std::size_t  = 0;
    virtual auto Write(std::vector<std::byte> data) -> std::size_t = 0;
    virtual auto Close() -> bool                                   = 0;
};
}  // namespace uranus::net
