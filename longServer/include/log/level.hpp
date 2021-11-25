#pragma once

#include <boost/algorithm/string.hpp>
#include <map>
#include <string_view>

namespace uranus::log {
//日志等级枚举类
enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NULL_LEVEL,
};

class Level
{
public:
    Level()  = default;
    ~Level() = default;

    //将日志等级解析成字符串
    static auto Marshal(const LogLevel &lvl) -> std::string {
        std::map<LogLevel, std::string> levelMap{
            {LogLevel::TRACE, "trace"},
            {LogLevel::DEBUG, "debug"},
            {LogLevel::INFO, "info"},
            {LogLevel::WARN, "warn"},
            {LogLevel::ERROR, "error"},
            {LogLevel::FATAL, "fatal"},
        };

        auto iter = levelMap.find(lvl);
        if (iter != levelMap.end()) {
            return iter->second;
        }
        return "";
    }

    //将字符串解析成日志等级
    static auto Unmarshal(std::string &str) -> LogLevel {
        boost::algorithm::to_lower(str);
        std::map<std::string, LogLevel> levelMap{{"trace", LogLevel::TRACE},
                                                 {"debug", LogLevel::DEBUG},
                                                 {"info", LogLevel::INFO},
                                                 {"warn", LogLevel::WARN},
                                                 {"error", LogLevel::ERROR},
                                                 {"fatal", LogLevel::FATAL}};
        auto                            iter = levelMap.find(str);
        if (iter != levelMap.end()) {
            return iter->second;
        }
        return LogLevel::NULL_LEVEL;
    }
};
}  // namespace uranus::log
