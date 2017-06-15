#pragma once

#include <string>
#include <ctime>

//时间操作
class TimeUtility
{
public:
	TimeUtility() = default;
	~TimeUtility() = default;

	static std::string GetCurrentTime();
	static std::string GetCurrentDay();

    static std::tm getNowTime();
};
