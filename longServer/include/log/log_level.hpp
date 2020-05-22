#pragma once

#include <boost/algorithm/string.hpp>
#include <map>
#include <string_view>

namespace Uranus::Log
{
//日志等级枚举类
enum class Level {
    TRACE,
    DEBUG,
    INFOR,
    WARN,
    ERROR,
    FATAL,
    NULL_LEVEL,
};

//将日志等级解析成字符串
inline auto Marshal(const Level &lvl) -> std::string
{
    std::map<Level, std::string> levelMap{
        {Level::TRACE, "trace"},
        {Level::DEBUG, "debug"},
        {Level::INFOR, "infor"},
        {Level::WARN, "warn"},
        {Level::ERROR, "error"},
        {Level::FATAL, "fatal"},
    };

    auto iter = levelMap.find(lvl);
    if (iter != levelMap.end()) {
        return iter->second;
    }
    return "";
}

//将字符串解析成日志等级
inline auto Unmarshal(std::string &str) -> Level
{
    boost::algorithm::to_lower(str);
    std::map<std::string, Level> levelMap{{"trace", Level::TRACE},
                                          {"debug", Level::DEBUG},
                                          {"infor", Level::INFOR},
                                          {"warn", Level::WARN},
                                          {"error", Level::ERROR},
                                          {"fatal", Level::FATAL}};
    auto iter = levelMap.find(str);
    if (iter != levelMap.end()) {
        return iter->second;
    }
    return Level::NULL_LEVEL;
}
}  // namespace Uranus::Log
