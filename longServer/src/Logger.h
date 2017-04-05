#pragma once


//日志等级枚举
enum class LogLevel{
    LOGSTDERR,
    LOGEMERG,
    LOGALERT,
    LOGCRIT,
    LOGERR,
    LOGWARN,
    LOGNOTICE,
    LOGINFO,
    LOGDEBUG,
};

class Logger
{
private:
    int nLogLevel; //日志等级

public:
    Logger();
    ~Logger();

    bool SetLevel();
};
