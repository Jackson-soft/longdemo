#pragma once

#include "request.hpp"
#include "response.hpp"
#include "types.hpp"
#include <string_view>

namespace Uranus::Http
{
class Client
{
public:
    Client()  = default;
    ~Client() = default;

    Response *Get(std::string_view rawurl)
    {
        if (rawurl.empty())
            return nullptr;
        auto req = newRequest(Uranus::Http::Method::Get, rawurl);

        return do();
    }

    void Post(std::string_view rawurl)
    {
        if (rawurl.empty())
            return;
    }

private:
    Response *do() { return nullptr; }
    Request *newRequest(Uranus::Http::Method method, std::string_view url) { return nullptr; }

private:
    int timeOut;
};
}  // namespace Uranus::Http
