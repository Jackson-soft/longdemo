#pragma once

#include "request.hpp"
#include "response.hpp"
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
    bool Get(const std::string_view rawurl)
    {
        if (rawurl.empty())
            return false;
        return false;
    }

    void Post(const std::string_view rawurl)
    {
        if (rawurl.empty())
            return;
    }

private:
    void do() {}

private:
    int timeOut;
};
}  // namespace Http
}  // namespace Uranus
