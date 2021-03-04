#pragma once

#include <string>

namespace uranus
{
namespace http
{
class Cookie
{
public:
    Cookie()  = default;
    ~Cookie() = default;

public:
    std::string &String() { return ""; }

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
}  // namespace http
}  // namespace uranus
