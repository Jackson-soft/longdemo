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

	static std::string GetCurrentTime()
	{
		auto NowTime = GetNowTime();
		return boost::str(boost::format("%s") %
						  std::put_time(&NowTime, "%F %T"));
	}

	static std::string GetCurrentDay()
	{
		auto NowTime = GetNowTime();
		return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F"));
	}

	static std::tm GetNowTime()
	{
		std::chrono::system_clock::time_point now =
			std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		return *std::localtime(&now_c);
	}
};
