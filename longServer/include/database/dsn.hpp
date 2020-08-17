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

    bool Parse(std::string_view dsn)
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
        mUser       = dsn.substr(0, tColon.begin() - dsn.begin());
        mPasswd     = dsn.substr(tColon.end() - dsn.begin(), tAt.begin() - tColon.end());

        // [protocol[(address)]]
        // Find the first '('
        auto tOpenParen = boost::find_first(dsn, "(");
        mNet            = dsn.substr(tAt.end() - dsn.begin(), tOpenParen.begin() - tAt.end());

        // dsn[i-1] must be == ')' if an address is specified
        auto tCloseParen = boost::find_last(dsn, ")");
        mAddress         = dsn.substr(tOpenParen.end() - dsn.begin(), tCloseParen.begin() - tOpenParen.end());

        if (mNet != "unix") {
            std::vector<std::string> vAddress;
            boost::split(vAddress, mAddress, boost::is_any_of(":"));

            if (!vAddress.empty() && (vAddress.size() == 2)) {
                mHost = vAddress.at(0);
                mPort = static_cast<std::uint32_t>(std::atoi(vAddress.at(1).data()));
            }
        }

        // dbname[?param1=value1&...&paramN=valueN]
        // Find the first '?'
        auto sParams   = dsn.substr(backslash.end() - dsn.begin(), dsn.end() - backslash.end());
        auto tQuestion = boost::find_first(sParams, "?");
        mDBName        = sParams.substr(0, tQuestion.begin() - sParams.begin());

        return parseParams(sParams.substr(tQuestion.end() - sParams.begin(), sParams.end() - tQuestion.end()));
    }

    [[nodiscard]] const std::string &User() const { return mUser; }

    [[nodiscard]] const std::string &Passwd() const { return mPasswd; }

    [[nodiscard]] const std::string &Net() const { return mNet; }

    const std::string &Address() { return mAddress; }

    [[nodiscard]] const std::string &Host() const { return mHost; }

    [[nodiscard]] std::uint32_t Port() const { return mPort; }

    [[nodiscard]] const std::string &DBName() const { return mDBName; }

    [[nodiscard]] const std::map<std::string, std::string> &Params() const { return mParams; }

private:
    bool parseParams(std::string_view params)
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
            mParams.emplace(vv.at(0), vv.at(1));
        }

        return true;
    }

    std::string mUser;
    std::string mPasswd;
    std::string mNet;  // 网络类型 tcp/unix
    std::string mAddress;
    std::string mHost;
    std::uint32_t mPort{};
    std::string mDBName;
    std::map<std::string, std::string> mParams;
};
}  // namespace Uranus::Database
