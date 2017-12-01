#include "Logger.h"

Logger::Logger(LogLevel level, std::string logPath, std::uint64_t maxFile)
    : mLevel(level), mPath(logPath), mMaxSize(maxFile), mBlockid(0),
      mIsChang(false)
{
	if (!std::experimental::filesystem::create_directory(logPath)) {
	}
}

Logger::~Logger()
{
    std::fclose(mFd);
    std::fclose(mChangFd);
}

void Logger::SetLevel(LogLevel logLevel) { mLevel = logLevel; }

Logger::LogLevel Logger::GetLevel() const { return mLevel; }

void Logger::Log(LogLevel level, const char *format, ...) {}

const char *Logger::logLevelToString(LogLevel level) const
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

void Logger::checkFile()
{
    auto nFileSize = std::experimental::filesystem::file_size(mLocation);

    if (nFileSize >= mMaxSize * 1024 * 1024) {
        mBlockid++;
        if (mCurrentDay != TimeUtil::GetCurrentDay()) {
			//序列重置为0
            mBlockid	= 0;
            mCurrentDay = TimeUtil::GetCurrentDay();
		}

        mLocation = boost::str(boost::format("%s/log_%s%d.log") % mPath %
                               mCurrentDay % mBlockid);

        mChangFd = std::fopen(mLocation.c_str(), "a");
        if (nullptr != mChangFd) {
            mIsChang = true;
			//设置buffer为line buffering
            std::setvbuf(mChangFd, nullptr, _IOLBF, 0);
		}
	}
}

bool Logger::openFile()
{
    mCurrentDay = TimeUtil::GetCurrentDay();
    mLocation   = boost::str(boost::format("%s/log_%s%d.log") % mPath %
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

int Logger::getLogHead(char *buffer, Logger::LogLevel level)
{
	return std::snprintf(buffer,
						 DEFAULT_BUFFER_SIZE,
						 "[%s] [%s] [%s:%d] [%s] ",
                         TimeUtil::GetCurrentTime().c_str(),
						 logLevelToString(level),
						 __FILE__,
						 __LINE__,
						 __FUNCTION__);
}
