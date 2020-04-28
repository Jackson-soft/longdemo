#pragma once

#include <string>
#include <unordered_map>

namespace Uranus::Http
{
class Response
{
public:
    Response()  = default;
    ~Response() = default;

private:
    int status;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
};

}  // namespace Uranus::Http
