#pragma once

#include <boost/format.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

//时间操作
class TimeUtil
{
public:
	TimeUtil()  = default;
	~TimeUtil() = default;

    // 获取当前时间 格式： YYYY-MM-DD HH:MM:SS
	static std::string GetCurrentTime()
	{
        auto NowTime = GetNowTime();
		return boost::str(boost::format("%s") %
						  std::put_time(&NowTime, "%F %T"));
	}

    // 获取当前日期 格式：YYYY-MM-DD
	static std::string GetCurrentDay()
	{
        auto NowTime = GetNowTime();
		return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F"));
	}

    // 获取当前时间戳
    static unsigned int GetUnixTime() {}

    static std::tm GetNowTime()
	{
		std::chrono::system_clock::time_point now =
			std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		return *std::localtime(&now_c);
	}
};
