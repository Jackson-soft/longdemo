#pragma once

#include <boost/algorithm/string.hpp>
#include <map>
#include <string_view>

namespace Uranus
{
namespace Log
{
//日志等级枚举类
enum class LogLevel {
    TRACE,
    DEBUG,
    INFOR,
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
    static std::string Marshal(const LogLevel &lvl)
    {
        switch (lvl) {
        case LogLevel::TRACE:
            return "trace";
        case LogLevel::DEBUG:
            return "debug";
        case LogLevel::INFOR:
            return "infor";
        case LogLevel::WARN:
            return "warn";
        case LogLevel::ERROR:
            return "error";
        case LogLevel::FATAL:
            return "fatal";
        default:
            return "";  // if the level is wrong
        }
    }

    //将字符串解析成日志等级
    static LogLevel Unmarshal(std::string &str)
    {
        boost::algorithm::to_lower(str);
        std::map<std::string, LogLevel> levelMap{{"trace", LogLevel::TRACE},
                                                 {"debug", LogLevel::DEBUG},
                                                 {"infor", LogLevel::INFOR},
                                                 {"warn", LogLevel::WARN},
                                                 {"error", LogLevel::ERROR},
                                                 {"fatal", LogLevel::FATAL}};
        return levelMap[str];
    }
};
}  // namespace Log
}  // namespace Uranus