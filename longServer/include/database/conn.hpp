#pragma once

#include <string_view>
namespace uranus::database {
class DBConn {
public:
    DBConn()                                     = default;
    virtual ~DBConn()                            = default;

    virtual void Execute(std::string_view query) = 0;
    virtual void Prepare(std::string_view query) = 0;
    virtual auto Ping() -> bool                  = 0;
    virtual auto Close() -> bool                 = 0;
};
}  // namespace uranus::database