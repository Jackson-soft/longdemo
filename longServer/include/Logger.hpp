#pragma once

#include "Backend.hpp"
#include "Formatter.hpp"
#include "LogLevel.hpp"
#include "Util.hpp"
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

// Logger 日志的主类
class Logger : public Noncopyable
{
public:
	Logger() {}

	~Logger() { std::fclose(mFd); }

	//单例实例
	static Logger *GetInstance()
	{
		static Logger logger;
		return &logger;
	}

	void SetLevel(LogLevel logLevel) { mLevel = logLevel; }

	LogLevel GetLevel() const { return mLevel; }

	//日志输出对外接口
	void Log(LogLevel level, const char *format, ...) {}

private:
	bool openFile()
	{
		mCurrentDay = TimeUtil::GetCurrentDay();
		mLocation   = boost::str(boost::format("%s/log_%s%d.log") % mLogPath %
								 mCurrentDay % mBlockid);

		mFd = std::fopen(mLocation.c_str(), "a");
		if (nullptr == mFd) {
			return false;
		}
		//设置buffer为line buffering
		if (std::setvbuf(mFd, nullptr, _IOLBF, 0) != 0) {
			return false;
		}
		return true;
	}

private:
	LogLevel mLevel;						   //日志等级
	std::string mLogPath;					   //日志存放目录
	std::uint64_t mMaxSize{500 * 1024 * 1024}; //日志分隔的大小上限
	std::FILE *mFd;							   //日志文件描述符
	int mBlockid{0};						   //日志的序号
	std::atomic_bool mIsChang{false};		   //是否要更换文件描述符
	std::string mLocation{""};				   //日志文件名
	std::string mCurrentDay;				   //当前日期
	uint64_t DEFAULT_BUFFER_SIZE{2 * 1024 * 1024};
};

#define LOG_INFO Logger::GetInstance().Log()
