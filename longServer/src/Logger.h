#pragma once

#include <string>
#include <cstdint>
#include <cstdio>

class Logger
{
public:
    //日志等级枚举类
    enum class LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    Logger(LogLevel level,std::string logPath, std::uint64_t maxFile);
    ~Logger();

    void SetLevel(LogLevel logLevel);
    LogLevel GetLevel() const;

    static Logger* GetInstance(LogLevel level, std::string logPath, std::uint64_t maxFile) {
        static Logger logger(level,logPath, maxFile);
        return &logger;
    }

private:
    const char* logLevelToString(LogLevel level) const;
    LogLevel stringToLogLevel(const std::string &logLevel) const;

    //检测日志文件是否超量
     void checkFile();


private:
    LogLevel tLogLevel; //日志等级
    std::string sLogPath; //日志存放目录
    std::uint64_t nMaxFileSize; //日志分隔的大小上限
    std::FILE* fFd; //日志文件描述符
    int nIndex; //日志的序号
};
