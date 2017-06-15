#include "Logger.h"

#include "TimeUtility.h"
#include <boost/format.hpp>
#include <cstdlib>
#include <experimental/filesystem>

Logger::Logger(LogLevel level, std::string logPath, std::uint64_t maxFile)
	: tLogLevel(level), sLogPath(logPath), nMaxFileSize(maxFile), nIndex(0)
{
	if (!std::experimental::filesystem::create_directory(logPath)) {
	}
	std::string fileName =
		boost::str(boost::format("%s/log_%s%d.log") % sLogPath %
				   TimeUtility::GetCurrentDay() % nIndex);

	fFd = std::fopen(fileName.c_str(), "w");
}

Logger::~Logger() { std::fclose(fFd); }

void Logger::SetLevel(LogLevel logLevel) { tLogLevel = logLevel; }

Logger::LogLevel Logger::GetLevel() const { return tLogLevel; }

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
