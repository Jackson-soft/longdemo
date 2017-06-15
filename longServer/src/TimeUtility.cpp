#include "TimeUtility.h"

#include <boost/format.hpp>
#include <chrono>
#include <iomanip>

/*
TimeUtility::TimeUtility() {}
TimeUtility::~TimeUtility() {}
*/

std::string TimeUtility::GetCurrentDay()
{
	std::tm NowTime = getNowTime();
	return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F"));
}

std::string TimeUtility::GetCurrentTime()
{
	std::tm NowTime = getNowTime();
	return boost::str(boost::format("%s") % std::put_time(&NowTime, "%F %T"));
}

std::tm TimeUtility::getNowTime()
{
	std::chrono::system_clock::time_point now =
		std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	return *std::localtime(&now_c);
}
