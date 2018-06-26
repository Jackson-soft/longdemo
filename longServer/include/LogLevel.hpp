#pragma once

#include <boost/algorithm/string.hpp>
#include <string_view>

//日志等级枚举类
enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
};

class Level
{
public:
	Level() {}

	//从字符串解析要严格一点
	Level(std::string_view str)
	{
		mLevel	= Unmarshal(str);
		mStrLevel = Marshal(mLevel);
	}

	Level(LogLevel lvl) : mLevel(lvl) { mStrLevel = Marshal(mLevel); }

	~Level() {}

	const std::string String() const { return mStrLevel; }

    LogLevel ToLevel() const { return mLevel; }

private:
	//将日志等级解析成字符串
	std::string Marshal(LogLevel lvl) const
	{
		switch (lvl) {
		case LogLevel::TRACE:
			return "trace";
		case LogLevel::DEBUG:
			return "debug";
		case LogLevel::INFO:
			return "info";
		case LogLevel::WARN:
			return "warn";
		case LogLevel::ERROR:
			return "error";
		case LogLevel::FATAL:
			return "fatal";
		default:
			return "info"; // if the level is wrong, use DEBUG instead
		}
	}

	//将字符串解析成日志等级
	LogLevel Unmarshal(std::string_view str) const
	{
        // boost::algorithm::to_lower(str);
		if (str == "trace")
			return LogLevel::TRACE;
		else if (str == "debgug")
			return LogLevel::DEBUG;
		else if (str == "info")
			return LogLevel::INFO;
		else if ("warn" == str)
			return LogLevel::WARN;
		else if ("error" == str)
			return LogLevel::ERROR;
		else if ("fatal" == str)
			return LogLevel::FATAL;
		else
			return LogLevel::INFO;
	}

private:
	std::string mStrLevel{""};
	LogLevel mLevel;
};
