#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Uranus::Utils
{
class LogHelper
{
public:
    static LogHelper &Get()
    {
        static LogHelper logger;
        return logger;
    }

    /*
     * lvl 日志等级
     * info warning error
     */
    bool SetLevel(const std::string &lvl)
    {
        auto tLevel = spdlog::level::from_str(lvl);
        if (tLevel == spdlog::level::off)
            return false;
        if (mLogger) {
            mLogger->set_level(tLevel);
            return true;
        }
        return false;
    }

    void OutPut(const spdlog::level::level_enum lvl, const std::string &msg, const std::string &file, const size_t line)
    {
        if (msg.empty())
            return;
        auto nIndex = file.find_last_of('/');

        // mLogger->log(lvl, fmt::format("[{}:{}] {}", file.substr(nIndex + 1), line, msg));
        mLogger->log(lvl, "[{}:{}] {}", file.substr(nIndex + 1), line, msg);
    }

private:
    LogHelper()
    {
        // docker 中日志要打在终端
        mLogger = spdlog::stdout_color_mt("console");
        mLogger->set_level(spdlog::level::info);
    }

    ~LogHelper() { spdlog::drop_all(); }

    LogHelper(const LogHelper &) = default;
    LogHelper(LogHelper &&)      = default;
    LogHelper &operator=(const LogHelper &) = default;
    LogHelper &operator=(LogHelper &&) = default;

private:
    std::shared_ptr<spdlog::logger> mLogger;
};

#define GLOG_INFO(msg) LogHelper::Get().OutPut(spdlog::level::info, msg, __FILE__, __LINE__)

#define GLOG_INFOF(fmter, args...)                                                                                     \
    LogHelper::Get().OutPut(spdlog::level::info, fmt::format(fmter, args), __FILE__, __LINE__)

#define GLOG_WARN(msg) LogHelper::Get().OutPut(spdlog::level::warn, msg, __FILE__, __LINE__)

#define GLOG_WARNF(fmter, args...)                                                                                     \
    LogHelper::Get().OutPut(spdlog::level::warn, fmt::format(fmter, args), __FILE__, __LINE__)

#define GLOG_ERROR(msg) LogHelper::Get().OutPut(spdlog::level::err, msg, __FILE__, __LINE__)

#define GLOG_ERRORF(fmter, args...)                                                                                    \
    LogHelper::Get().OutPut(spdlog::level::err, fmt::format(fmter, args), __FILE__, __LINE__)

}  // namespace Uranus