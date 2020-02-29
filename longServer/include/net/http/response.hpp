#pragma once

#include <string>
#include <unordered_map>

namespace Uranus
{
namespace Http
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

}  // namespace Http
}  // namespace Uranus
