#pragma once

#include <string>

namespace Uranus::Http
{
class Cookie
{
public:
    Cookie()  = default;
    ~Cookie() = default;

    std::string String() { return ""; }

private:
    std::string name;
    std::string value;
    std::string path;
    std::string domain;
    int maxAge;
    bool secure;
    bool httpOnly;
    std::string raw;
};
}  // namespace Uranus::Http
