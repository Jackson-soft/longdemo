#pragma once

#include "request.hpp"
#include "response.hpp"
#include "types.hpp"
#include <string_view>

namespace uranus
{
namespace http
{
class Client
{
public:
    Client()  = default;
    ~Client() = default;

public:
    Response *Get(const std::string_view rawurl)
    {
        if (rawurl.empty())
            return nullptr;
        auto req = newRequest(uranus::http::Method::Get, rawurl);

        return do();
    }

    void Post(const std::string_view rawurl)
    {
        if (rawurl.empty())
            return;
    }

private:
    Response *do() { return nullptr; }
    Request *newRequest(uranus::http::Method method, const std::string_view url) { return nullptr; }

private:
    int timeOut;
};
}  // namespace http
}  // namespace uranus
