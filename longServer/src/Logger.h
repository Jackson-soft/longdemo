#pragma once

#include "TimeUtil.hpp"
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

    Logger() = default;
    Logger(LogLevel level, std::string logPath, std::uint64_t maxFile);
    ~Logger();

    void SetLevel(LogLevel logLevel);
    LogLevel GetLevel() const;

    //单例实例
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
    LogLevel mLevel;		   //日志等级
    std::string mPath;		   //日志存放目录
    std::uint64_t mMaxSize;	//日志分隔的大小上限
    std::FILE *mFd;			   //日志文件描述符
    std::FILE *mChangFd;	   //日志文件描述符
    int mBlockid;			   //日志的序号
    std::atomic_bool mIsChang; //是否要更换文件描述符
    std::string mLocation;	 //日志文件名
    std::string mCurrentDay;   //当前日期
    uint64_t DEFAULT_BUFFER_SIZE{2 * 1024 * 1024};
};
