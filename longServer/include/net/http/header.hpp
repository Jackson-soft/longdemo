#pragma once

#include <map>
#include <string>
#include <string_view>

namespace Uranus::Http
{
class Header
{
public:
    Header()  = default;
    ~Header() = default;

    void Add(std::string_view key, std::string_view value) {}
    void Delete(std::string_view key) {}
    std::string &Get(std::string_view key) { return ""; }
    void Set(std::string_view key, std::string_view value) {}

private:
    std::map<std::string, std::string> header;
};
}  // namespace Uranus::Http
