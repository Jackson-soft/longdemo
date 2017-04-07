#pragma once


class Logger
{
public:
    //日志等级枚举
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

    Logger();
    ~Logger();

    bool SetLevel(LogLevel logLevel);
    LogLevel GetLevel();



private:
    LogLevel nLogLevel; //日志等级
};
