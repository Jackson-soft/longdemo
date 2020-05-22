#pragma once

#include "backend.hpp"
#include "formatter.hpp"
#include "log_level.hpp"
#include "utils/noncopyable.hpp"
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>

namespace Uranus::Log
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
    static auto Get() -> std::shared_ptr<Logger>
    {
        static auto logger = std::make_shared<Logger>();
        return logger;
    }

    //初始化，只需要调一次
    auto Set(const Level lvl, std::unique_ptr<Formatter> fmt, std::unique_ptr<Backend> backend) -> bool
    {
        std::lock_guard<std::mutex> locker(mutex);
        level = lvl;
        formatter.swap(fmt);
        backend.swap(backend);
        return true;
    }

    //日志输出对外接口
    void Inforln(std::string_view msg) { return outPut(Level::INFOR, msg); }

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
    void outPut(const Level lvl, std::string_view msg)
    {
        std::lock_guard<std::mutex> locker(mutex);
        if (lvl >= level) {
            buffer.push(formatter->Format(msg));
        }
    }

private:
    std::mutex mutex;
    Level level;
    std::unique_ptr<Formatter> formatter;  //格式化前端
    std::unique_ptr<Backend> backend;      //输出后端
    std::queue<std::string> buffer;        //环型缓存
};

#define LOG_INFO Logger::GetInstance().Inforln()
}  // namespace Uranus::Log
