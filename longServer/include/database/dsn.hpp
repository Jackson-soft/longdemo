#pragma once

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

/*  解析DSN类
 *
 *  data:data@tcp(10.1.6.134:3306)/guard?charset=utf8mb4&loc=Asia%2FChongqing
 *
 *  [user[:password]@][net[(addr)]]/dbname[?param1=value1&paramN=valueN]
 *
 *  username:password@protocol(address)/dbname?param=value
 *
 */

namespace Uranus::Database
{
class DSN
{
public:
    DSN()  = default;
    ~DSN() = default;

    auto Parse(const std::string_view dsn) -> bool
    {
        if (dsn.empty())
            return false;

        // [user[:password]@][net[(addr)]]/dbname[?param1=value1&paramN=valueN]
        // Find the last '/' (since the password or the net addr might contain a '/')
        auto backslash = boost::find_last(dsn, "/");

        // [username[:password]@][protocol[(address)]]
        // Find the last '@'
        auto tAt = boost::find_last(dsn, "@");

        // username[:password]
        // Find the first ':'
        auto tColon = boost::find_first(dsn, ":");
        user        = dsn.substr(0, tColon.begin() - dsn.begin());
        passwd      = dsn.substr(tColon.end() - dsn.begin(), tAt.begin() - tColon.end());

        // [protocol[(address)]]
        // Find the first '('
        auto tOpenParen = boost::find_first(dsn, "(");
        network         = dsn.substr(tAt.end() - dsn.begin(), tOpenParen.begin() - tAt.end());

        // dsn[i-1] must be == ')' if an address is specified
        auto tCloseParen = boost::find_last(dsn, ")");
        address          = dsn.substr(tOpenParen.end() - dsn.begin(), tCloseParen.begin() - tOpenParen.end());

        if (network != "unix") {
            std::vector<std::string> vAddress;
            boost::split(vAddress, address, boost::is_any_of(":"));

            if (!vAddress.empty() && (vAddress.size() == 2)) {
                host = vAddress.at(0);
                port = static_cast<std::uint16_t>(std::atoi(vAddress.at(1).data()));
            }
        }

        // dbname[?param1=value1&...&paramN=valueN]
        // Find the first '?'
        auto sParams   = dsn.substr(backslash.end() - dsn.begin(), dsn.end() - backslash.end());
        auto tQuestion = boost::find_first(sParams, "?");
        DBName         = sParams.substr(0, tQuestion.begin() - sParams.begin());

        return parseParams(sParams.substr(tQuestion.end() - sParams.begin(), sParams.end() - tQuestion.end()));
    }

    [[nodiscard]] const std::string &User() const { return user; }

    [[nodiscard]] const std::string &Passwd() const { return passwd; }

    [[nodiscard]] const std::string &Net() const { return network; }

    const std::string &Address() { return address; }

    [[nodiscard]] const std::string &Host() const { return host; }

    [[nodiscard]] std::uint16_t Port() const { return port; }

    [[nodiscard]] const std::string &DBName() const { return DBName; }

    [[nodiscard]] const std::map<std::string, std::string> &Params() const { return params; }

private:
    auto parseParams(const std::string_view params) -> bool
    {
        if (params.empty())
            return false;
        std::vector<std::string> vParam;
        boost::split(vParam, params, boost::is_any_of("&"));
        if (vParam.empty())
            return false;

        for (const auto &it : vParam) {
            std::vector<std::string> vv;
            boost::split(vv, it, boost::is_any_of("="));
            if (vv.size() != 2)
                continue;
            params.emplace(vv.at(0), vv.at(1));
        }

        return true;
    }

    std::string user;
    std::string passwd;
    std::string network;  // 网络类型 tcp/unix
    std::string address;
    std::string host;
    std::uint16_t port{};
    std::string DBName;
    std::map<std::string, std::string> params;
};
}  // namespace Uranus::Database
