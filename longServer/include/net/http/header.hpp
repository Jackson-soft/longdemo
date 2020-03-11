#pragma once

#include <map>
#include <string>
#include <string_view>

namespace Uranus
{
namespace Http
{
class Header
{
public:
    Header()  = default;
    ~Header() = default;

public:
    void Add(const std::string_view key, const std::string_view value) {}
    void Delete(const std::string_view key) {}
    std::string &Get(const std::string_view key) { return ""; }
    void Set(const std::string_view key, const std::string_view value) {}

private:
    std::map<std::string, std::string> header;
};
}  // namespace Http
}  // namespace Uranus
