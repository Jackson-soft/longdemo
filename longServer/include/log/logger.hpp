#pragma once

#include "backend.hpp"
#include "formatter.hpp"
#include "log_level.hpp"
#include "utils/util.hpp"
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>

namespace Uranus
{
namespace Log
{
// Logger 日志的主类
class Logger : public Utils::Noncopyable
{
public:
    Logger() = default;

    ~Logger()
    {
        auto fpt = formatter.release();
        delete fpt;
        auto fbt = backend.release();
        delete fbt;
    }

    //单例实例
    static std::shared_ptr<Logger> GetInstance()
    {
        static auto logger = std::make_shared<Logger>();
        return logger;
    }

    //初始化，只需要调一次
    bool Set(const LogLevel &lvl, std::unique_ptr<Formatter> fmt, std::unique_ptr<Backend> backend)
    {
        std::lock_guard<std::mutex> locker(mutex);
        level = lvl;
        formatter.swap(fmt);
        backend.swap(backend);
        return true;
    }

    //日志输出对外接口
    void Inforln(const std::string_view msg) { return outPut(LogLevel::INFOR, msg); }

    void Run()
    {
        while (true) {
            if (buffer.size() > 0) {
                backend->Write(buffer.front());
                buffer.pop();
            }
        }
    }

private:
    //输出
    void outPut(LogLevel level, std::string_view msg)
    {
        std::lock_guard<std::mutex> locker(mutex);
        if (level >= level) {
            buffer.push(formatter->Format(msg));
        }
    }

private:
    std::mutex mutex;
    LogLevel level;
    std::unique_ptr<Formatter> formatter;  //格式化前端
    std::unique_ptr<Backend> backend;      //输出后端
    std::queue<std::string> buffer;        //环型缓存
};

#define LOG_INFO Logger::GetInstance().Inforln()
}  // namespace Log
}  // namespace Uranus