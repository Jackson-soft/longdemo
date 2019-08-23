#pragma once

#include <boost/format.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

namespace Uranus
{
//时间操作
class TimeUtil
{
public:
    TimeUtil()  = default;
    ~TimeUtil() = default;

    // 获取当前时间 格式： YYYY-MM-DD HH:MM:SS
    static std::string CurrentTime()
    {
        auto now = NowTime();
        return boost::str(boost::format("%s") % std::put_time(&now, "%F %T"));
    }

    // 获取当前日期 格式：YYYY-MM-DD
    static std::string CurrentDay()
    {
        auto now = NowTime();
        return boost::str(boost::format("%s") % std::put_time(&now, "%F"));
    }

    // 获取当前时间戳(秒精度)
    static std::time_t UnixTime()
    {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp
            = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

        auto tmp = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());

        std::time_t timeTamp = tmp.count();
        return timeTamp;
    }

    // 获取当前时间戳(毫秒精度)
    static std::time_t MilUnixTime()
    {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp
            = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

        auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());

        std::time_t timeTamp = tmp.count();
        return timeTamp;
    }

    static std::tm NowTime()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c                         = std::chrono::system_clock::to_time_t(now);
        return *std::localtime(&now_c);
    }
};
}  // namespace Uranus