#pragma once

#include <map>
#include <string>
#include <string_view>

// [scheme:][//[userinfo@]host][/]path[?query][#fragment]

namespace Uranus
{
namespace Http
{
class URL
{
public:
    URL()  = default;
    ~URL() = default;

public:
    auto Parse(std::string_view rawurl) -> bool
    {
        if (rawurl.empty())
            return false;
        if (rawurl == "*") {
            path_ = "*";
            return true;
        }
        return false;
    }
    bool Query() { return false; }

    auto IsAbs() -> bool { return !scheme_.empty(); }

private:
    std::string scheme_;
    std::string opaque_;  // encoded opaque data
    std::string host_;    // host or host:port
    std::string path_;
    std::string rawQuery_;
    std::string fragment_;
};
}  // namespace Http
}  // namespace Uranus
