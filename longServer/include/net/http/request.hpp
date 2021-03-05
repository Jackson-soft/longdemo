#pragma once

#include "url.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

namespace uranus::http
{
class Request
{
public:
    Request()  = default;
    ~Request() = default;

    std::string cookie(std::string_view name)
    {
        if (name.empty())
            return "";
        return "";
    }
    void addCookie() {}

private:
    std::string method_;
    URL *url_;
    int proto_;  // Version
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
    std::string host_;
    std::unordered_map<std::string, std::string> form_;
};
}  // namespace uranus::http
