#pragma once

#include <boost/algorithm/string.hpp>
#include <string_view>

namespace Uranus
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
    static std::string Marshal(const LogLevel lvl)
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
        if (str == "trace")
            return LogLevel::TRACE;
        else if (str == "debgug")
            return LogLevel::DEBUG;
        else if (str == "infor")
            return LogLevel::INFOR;
        else if ("warn" == str)
            return LogLevel::WARN;
        else if ("error" == str)
            return LogLevel::ERROR;
        else if ("fatal" == str)
            return LogLevel::FATAL;
        else
            return LogLevel::NULL_LEVEL;
    }
};
}  // namespace Uranus