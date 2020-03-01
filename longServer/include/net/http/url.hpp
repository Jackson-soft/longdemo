#pragma once

#include <map>
#include <string>
#include <string_view>

/* [scheme:][//[userinfo@]host][/]path[?query][#fragment]
 * RFC 3986[https://tools.ietf.org/pdf/rfc3986.pdf]
 *
                     hierarchical part
        ┌───────────────────┴─────────────────────┐
                    authority               path
        ┌───────────────┴───────────────┐┌───┴────┐
  abc://username:password@example.com:123/path/data?key=value&key2=value2#fragid1
  └┬┘   └───────┬───────┘ └────┬────┘ └┬┘           └─────────┬─────────┘ └──┬──┘
scheme  user information     host     port                  query         fragment

  urn:example:mammal:monotreme:echidna
  └┬┘ └──────────────┬───────────────┘
scheme              path

*/

namespace Uranus
{
namespace Http
{
class URL
{
public:
    URL()  = default;
    ~URL() = default;

public:
    auto Parse(std::string_view rawurl) -> bool
    {
        if (rawurl.empty())
            return false;
        if (rawurl == "*") {
            path_ = "*";
            return true;
        }
        return false;
    }
    bool Query() { return false; }

    auto IsAbs() -> bool { return !scheme_.empty(); }

private:
    std::string scheme_;
    std::string opaque_;  // encoded opaque data
    std::string host_;    // host or host:port
    std::string path_;
    std::string rawQuery_;
    std::string fragment_;
};
}  // namespace Http
}  // namespace Uranus
