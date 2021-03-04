#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/split.hpp>
#include <map>
#include <string>
#include <string_view>
#include <vector>

/* [scheme:][//[userinfo@]host][/]path[?query][#fragment]
  RFC 3986[https://tools.ietf.org/pdf/rfc3986.pdf]

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

namespace uranus::http
{
class URL
{
public:
    URL()  = default;
    ~URL() = default;

public:
    auto parse(const std::string_view rawurl) -> bool
    {
        if (rawurl.empty())
            return false;
        if (rawurl == "*") {
            path = "*";
            return true;
        }

        std::vector<std::string> result;
        result.reserve(2);

        boost::split(result, rawurl, boost::is_any_of("#"), boost::token_compress_on);
        fragment = result[1];
        result.clear();
        boost::split(result, rawurl, boost::is_any_of("://"), boost::token_compress_on);
        scheme = boost::to_lower_copy(result[0]);

        boost::to_lower(scheme);

        return false;
    }
    bool query() { return false; }

    auto isAbs() -> bool { return !scheme.empty(); }

    //
    std::string &string() { return ""; }

private:
    std::string scheme;
    std::string opaque;  // encoded opaque data
    std::string host;    // host or host:port
    std::string path;
    std::string rawQuery;
    std::string fragment;
};
}  // namespace uranus::http
