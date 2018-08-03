#pragma once

#include "Backend.hpp"
#include "Formatter.hpp"
#include "Util.hpp"
#include <LogLevel.hpp>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
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

    void Init(std::string_view level,
              std::string_view logPath,
              std::uint64_t maxSize)
	{
        if (!std::filesystem::create_directory(logPath)) {
		}
		mLevel   = level;
		mLogPath = logPath;
		mMaxSize = maxSize;
	}

	void SetLevel(LogLevel logLevel) { mLevel = logLevel; }

	LogLevel GetLevel() const { return mLevel; }

	//日志输出对外接口
	void Log(LogLevel level, const char *format, ...) {}

private:
	//检测日志文件是否超量
	void checkFile()
	{
		auto nFileSize = std::experimental::filesystem::file_size(mLocation);

		if (nFileSize >= mMaxSize * 1024 * 1024) {
			mBlockid++;
			if (mCurrentDay != TimeUtil::GetCurrentDay()) {
				//序列重置为0
				mBlockid	= 0;
				mCurrentDay = TimeUtil::GetCurrentDay();
			}

			mLocation = boost::str(boost::format("%s/log_%s%d.log") % mLogPath %
								   mCurrentDay % mBlockid);

			mChangFd = std::fopen(mLocation.c_str(), "a");
			if (nullptr != mChangFd) {
				mIsChang = true;
				//设置buffer为line buffering
				std::setvbuf(mChangFd, nullptr, _IOLBF, 0);
			}
		}
	}

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
