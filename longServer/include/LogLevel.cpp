#pragma once

#include <boost/algorithm/string.hpp>
#include <string_view>

class Level
{
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

public:
	Level() {}
	~Level() {}

private:
	const char *String(LogLevel level) const
	{
		switch (level) {
		case LogLevel::TRACE:
			return "TRACE";
		case LogLevel::DEBUG:
			return "DEBUG";
		case LogLevel::INFO:
			return "INFO";
		case LogLevel::WARN:
			return "WARN";
		case LogLevel::ERROR:
			return "ERROR";
		case LogLevel::FATAL:
			return "FATAL";
		default:
			return "DEBUG"; // if the level is wrong, use DEBUG instead
		}
	}

	//将字符串解析成日志等级
	LogLevel ParseLevel(std::string_view &logLevel)
	{
		boost::to_lower(logLevel);
		if (logLevel == "TRACE")
			return LogLevel::TRACE;
		else if (logLevel == "DEBUG")
			return LogLevel::DEBUG;
		else if (logLevel == "INFO")
			return LogLevel::INFO;
		else if ("WARN" == logLevel)
			return LogLevel::WARN;
		else if ("ERROR" == logLevel)
			return LogLevel::ERROR;
		else if ("FATAL" == logLevel)
			return LogLevel::FATAL;
		else
			return LogLevel::INFO;
	}
};