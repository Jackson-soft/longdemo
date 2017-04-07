#include "Logger.h"


Logger::Logger()
{

}

Logger::~Logger()
{

}

bool Logger::SetLevel(LogLevel logLevel)
{
    nLogLevel = logLevel;
    return true;
}

Logger::LogLevel Logger::GetLevel()
{
    return nLogLevel;
}
