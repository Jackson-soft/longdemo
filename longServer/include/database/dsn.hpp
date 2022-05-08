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

namespace uranus::database {
class DSN {
public:
    DSN()  = default;
    ~DSN() = default;

    auto Parse(std::string_view dsn) -> bool {
        if (dsn.empty()) {
            return false;
        }

        // [user[:password]@][net[(addr)]]/dbname[?param1=value1&paramN=valueN]
        // Find the last '/' (since the password or the net addr might contain a '/')
        auto backslash   = boost::find_last(dsn, "/");

        // [username[:password]@][protocol[(address)]]
        // Find the last '@'
        auto tAt         = boost::find_last(dsn, "@");

        // username[:password]
        // Find the first ':'
        auto tColon      = boost::find_first(dsn, ":");
        user_            = dsn.substr(0, tColon.begin() - dsn.begin());
        password_        = dsn.substr(tColon.end() - dsn.begin(), tAt.begin() - tColon.end());

        // [protocol[(address)]]
        // Find the first '('
        auto tOpenParen  = boost::find_first(dsn, "(");
        network_         = dsn.substr(tAt.end() - dsn.begin(), tOpenParen.begin() - tAt.end());

        // dsn[i-1] must be == ')' if an address is specified
        auto tCloseParen = boost::find_last(dsn, ")");
        address_         = dsn.substr(tOpenParen.end() - dsn.begin(), tCloseParen.begin() - tOpenParen.end());

        if (network_ != "unix") {
            std::vector<std::string> vAddress;
            boost::split(vAddress, address_, boost::is_any_of(":"));

            if (!vAddress.empty() && (vAddress.size() == 2)) {
                host_ = vAddress.at(0);
                port_ = static_cast<std::uint32_t>(std::atoi(vAddress.at(1).data()));
            }
        }

        // dbname[?param1=value1&...&paramN=valueN]
        // Find the first '?'
        auto sParams   = dsn.substr(backslash.end() - dsn.begin(), dsn.end() - backslash.end());
        auto tQuestion = boost::find_first(sParams, "?");
        schema_        = sParams.substr(0, tQuestion.begin() - sParams.begin());

        return parseParams(sParams.substr(tQuestion.end() - sParams.begin(), sParams.end() - tQuestion.end()));
    }

    [[nodiscard]] auto User() const -> const std::string & {
        return user_;
    }

    [[nodiscard]] auto Passwd() const -> const std::string & {
        return password_;
    }

    [[nodiscard]] auto net() const -> const std::string & {
        return network_;
    }

    auto Address() -> const std::string & {
        return address_;
    }

    [[nodiscard]] auto Host() const -> const std::string & {
        return host_;
    }

    [[nodiscard]] auto Port() const -> std::uint32_t {
        return port_;
    }

    [[nodiscard]] auto DBName() const -> const std::string & {
        return schema_;
    }

    [[nodiscard]] auto Params() const -> const std::map<std::string, std::string> & {
        return params_;
    }

private:
    auto parseParams(std::string_view params) -> bool {
        if (params.empty()) {
            return false;
        }
        std::vector<std::string> vParam;
        boost::split(vParam, params, boost::is_any_of("&"));
        if (vParam.empty()) {
            return false;
        }

        for (const auto &it : vParam) {
            std::vector<std::string> vv;
            boost::split(vv, it, boost::is_any_of("="));
            if (vv.size() != 2) {
                continue;
            }
            params_.emplace(vv.at(0), vv.at(1));
        }

        return true;
    }

    std::string                        user_;
    std::string                        password_;
    std::string                        network_;  // 网络类型 tcp/unix
    std::string                        address_;
    std::string                        host_;
    std::uint32_t                      port_{};
    std::string                        schema_;
    std::map<std::string, std::string> params_;
};
}  // namespace uranus::database
