#pragma once

#include "request.hpp"
#include "response.hpp"
#include "types.hpp"

#include <string_view>

namespace uranus::http {
class Client {
public:
    Client()  = default;
    ~Client() = default;

    Response *get(const std::string_view rawurl) {
        if (rawurl.empty()) {
            return nullptr;
        }
        auto req = newRequest(uranus::http::Method::Get, rawurl);

        return do();
    }

    void post(const std::string_view rawurl) {
        if (rawurl.empty())
            return;
    }

private:
    Response *do() {
        return nullptr;
    }

    Request *newRequest(uranus::http::Method method, const std::string_view url) {
        return nullptr;
    }

    int timeout_;
};
}  // namespace uranus::http
