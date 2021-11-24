#pragma once

#include <string>
#include <unordered_map>

namespace uranus::http {
class Response {
public:
    Response()  = default;
    ~Response() = default;

private:
    int                                          statusCode_;
    std::string                                  body_;
    std::unordered_map<std::string, std::string> headers_;
};

}  // namespace uranus::http
