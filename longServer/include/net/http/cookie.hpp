#pragma once

#include <string>

namespace Uranus
{
namespace Http
{
class Cookie
{
public:
    Cookie()  = default;
    ~Cookie() = default;

private:
    std::string name_;
    std::string value_;
};
}  // namespace Http
}  // namespace Uranus
