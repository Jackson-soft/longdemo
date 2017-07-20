#pragma once

#include <ctime>
#include <string>

//时间操作
class TimeUtil
{
public:
	TimeUtil()  = default;
	~TimeUtil() = default;

	static std::string GetCurrentTime();
	static std::string GetCurrentDay();

	static std::tm GetNowTime();
};
