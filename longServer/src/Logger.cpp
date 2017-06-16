#include "Logger.h"

Logger::Logger(LogLevel level, std::string logPath, std::uint64_t maxFile)
	: tLogLevel(level), sLogPath(logPath), nMaxFileSize(maxFile), nLogBlockid(0)
{
	if (!std::experimental::filesystem::create_directory(logPath)) {
	}
	bChangFd = false;
}

Logger::~Logger() { std::fclose(fFd); }

void Logger::SetLevel(LogLevel logLevel) { tLogLevel = logLevel; }

Logger::LogLevel Logger::GetLevel() const { return tLogLevel; }

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
	auto nFileSize = std::experimental::filesystem::file_size(sLogLocation);

	if (nFileSize >= nMaxFileSize * 1024 * 1024) {
		nLogBlockid++;
		if (sCurrentDay != TimeUtility::GetCurrentDay()) {
			//序列重置为0
			nLogBlockid = 0;
			sCurrentDay = TimeUtility::GetCurrentDay();
		}

		sLogLocation = boost::str(boost::format("%s/log_%s%d.log") % sLogPath %
								  sCurrentDay % nLogBlockid);

		fChangFd = std::fopen(sLogLocation.c_str(), "a");
		if (nullptr != fChangFd) {
			bChangFd = true;
			//设置buffer为line buffering
			std::setvbuf(fChangFd, nullptr, _IOLBF, 0);
		}
	}
}

bool Logger::openFile()
{
	sCurrentDay  = TimeUtility::GetCurrentDay();
	sLogLocation = boost::str(boost::format("%s/log_%s%d.log") % sLogPath %
							  sCurrentDay % nLogBlockid);

	fFd = std::fopen(sLogLocation.c_str(), "a");
	if (nullptr == fFd) {
		return false;
	}
	//设置buffer为line buffering
	if (std::setvbuf(fFd, nullptr, _IOLBF, 0) != 0) {
		return false;
	}
	return true;
}

int Logger::getLogHead(char *buffer, Logger::LogLevel level)
{
	return std::snprintf(buffer,
						 DEFAULT_BUFFER_SIZE,
						 "[%s] [%s] [%s:%d] [%s] ",
						 TimeUtility::GetCurrentTime().c_str(),
						 logLevelToString(level),
						 __FILE__,
						 __LINE__,
						 __FUNCTION__);
}
