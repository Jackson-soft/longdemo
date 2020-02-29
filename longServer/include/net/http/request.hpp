#pragma once

#include "url.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

namespace Uranus
{
namespace Http
{
class Request
{
public:
    Request()  = default;
    ~Request() = default;

public:
    std::string Cookie(const std::string_view name) { return ""; }
    void AddCookie() {}

private:
    std::string method_;
    URL *url_;
    int proto_;  // Version
    std::unordered_map<std::string, std::string> headers;
    std::string body_;
    std::string host_;
    std::unordered_map<std::string, std::string> form_;
};
}  // namespace Http
}  // namespace Uranus
