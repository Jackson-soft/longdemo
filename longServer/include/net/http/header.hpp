#pragma once

#include <map>
#include <string>
#include <string_view>

namespace uranus::http
{
class Header
{
public:
    Header()  = default;
    ~Header() = default;

    void add(std::string_view key, std::string_view value) {}
    void delete (std::string_view key) {}
    std::string &get(std::string_view key) { return ""; }
    void set(std::string_view key, std::string_view value) {}

private:
    std::map<std::string, std::string> header_;
};
}  // namespace uranus::http
