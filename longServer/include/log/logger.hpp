#pragma once

#include "backend.hpp"
#include "formatter.hpp"
#include "level.hpp"
#include "utils/noncopyable.hpp"
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>

namespace uranus::log
{
// Logger 日志的主类
class Logger: public utils::Noncopyable
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
    static std::shared_ptr<Logger> get()
    {
        static auto Logger = std::make_shared<Logger>();
        return Logger;
    }

    //初始化，只需要调一次
    bool set(const logLevel &lvl, std::unique_ptr<Formatter> fmt, std::unique_ptr<Backend> backend)
    {
        std::lock_guard<std::mutex> locker(mutex);
        level = lvl;
        formatter.swap(fmt);
        backend.swap(backend);
        return true;
    }

    //日志输出对外接口
    void infoln(std::string_view msg) { return outPut(LogLevel::INFOR, msg); }

    void run()
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
    void outPut(logLevel lvl, std::string_view msg)
    {
        std::lock_guard<std::mutex> locker(mutex);
        if (lvl >= level) {
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

#define LOG_INFO Logger::GetInstance().infoln()
}  // namespace uranus::log
