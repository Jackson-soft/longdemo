#pragma once

#include <string_view>

namespace Uranus
{
namespace Http
{
class Client
{
public:
    Client()  = default;
    ~Client() = default;

public:
    bool Get(const std::string_view rawurl) { return false; }
    void Post(const std::string_view rawurl) {}

private:
    void do() {}

private:
    int timeOut;
};
}  // namespace Http
}  // namespace Uranus
