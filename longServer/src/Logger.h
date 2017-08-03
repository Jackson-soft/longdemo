#pragma once

#include "TimeUtil.h"
#include <atomic>
#include <boost/format.hpp>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <experimental/filesystem>
#include <string>

class Logger
{
public:
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

    Logger(LogLevel level, std::string logPath, std::uint64_t maxFile);
    ~Logger();

    void SetLevel(LogLevel logLevel);
    LogLevel GetLevel() const;

    static Logger *
    GetInstance(LogLevel level, std::string logPath, std::uint64_t maxFile)
    {
        static Logger logger(level, logPath, maxFile);
        return &logger;
    }
    //日志输出对外接口
    void Log(LogLevel level, const char *format, ...);

private:
    const char *logLevelToString(LogLevel level) const;
    LogLevel stringToLogLevel(const std::string &logLevel) const;

    //检测日志文件是否超量
    void checkFile();

    bool openFile();

    //组装日志头
    int getLogHead(char *buffer, LogLevel level);

private:
    LogLevel tLogLevel;			//日志等级
    std::string sLogPath;		//日志存放目录
    std::uint64_t nMaxFileSize; //日志分隔的大小上限
    std::FILE *fFd;				//日志文件描述符
    std::FILE *fChangFd;		//日志文件描述符
    std::atomic_bool bChangFd;  //是否要更换文件描述符
    int nLogBlockid;			//日志的序号
    std::string sLogLocation;   //日志文件名
    std::string sCurrentDay;	//当前日期
    uint64_t DEFAULT_BUFFER_SIZE = 2 * 1024 * 1024;
};
