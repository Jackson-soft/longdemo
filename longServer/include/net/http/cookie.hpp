#pragma once

#include <string>

namespace uranus::http {
class Cookie {
public:
    Cookie()  = default;
    ~Cookie() = default;

public:
    std::string &string()
    {
        return "";
    }

private:
    std::string name;
    std::string value;
    std::string path;
    std::string domain;
    int         maxAge;
    bool        secure;
    bool        httpOnly;
    std::string raw;
};
}  // namespace uranus::http
