#pragma once

#include <string>
#include <unordered_map>

namespace uranus
{
namespace http
{
class Response
{
public:
    Response()  = default;
    ~Response() = default;

private:
    int statusCode;
    std::string body;
    std::unordered_map<std::string, std::string> headers_;
};

}  // namespace http
}  // namespace uranus
