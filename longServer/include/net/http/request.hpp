#pragma once

#include "url.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

namespace Uranus::Http
{
class Request
{
public:
    Request()  = default;
    ~Request() = default;

    auto Cookie(std::string_view name) -> std::string
    {
        if (name.empty())
            return "";
        return "";
    }
    void AddCookie() {}

private:
    std::string method;
    URL *url;
    int proto;  // Version
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string host;
    std::unordered_map<std::string, std::string> form;
};
}  // namespace Uranus::Http
