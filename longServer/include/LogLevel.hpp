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
    Level(std::string_view lvl) : mLevel(lvl) {}
	~Level() {}

private:
    const char *String() const
	{
		switch (level) {
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
			return "debug"; // if the level is wrong, use DEBUG instead
		}
	}

	//将字符串解析成日志等级
	LogLevel ParseLevel(std::string_view &logLevel)
	{
		boost::algorithm::to_lower(logLevel);
		if (logLevel == "trace")
			return LogLevel::TRACE;
		else if (logLevel == "debgug")
			return LogLevel::DEBUG;
		else if (logLevel == "info")
			return LogLevel::INFO;
		else if ("warn" == logLevel)
			return LogLevel::WARN;
		else if ("error" == logLevel)
			return LogLevel::ERROR;
		else if ("fatal" == logLevel)
			return LogLevel::FATAL;
		else
			return LogLevel::INFO;
	}

private:
    std::string mLevel{""};
};
