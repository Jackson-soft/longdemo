#include "TimeUtil.h"

#include <boost/format.hpp>
#include <chrono>
#include <iomanip>

/*
TimeUtil::TimeUtil() {}
TimeUtil::~TimeUtil() {}
*/

std::string TimeUtil::GetCurrentDay()
{
	auto NowTime = GetNowTime();
	return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F"));
}

std::string TimeUtil::GetCurrentTime()
{
	auto NowTime = GetNowTime();
	return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F %T"));
}

std::tm TimeUtil::GetNowTime()
{
	std::chrono::system_clock::time_point now =
		std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	return *std::localtime(&now_c);
}
