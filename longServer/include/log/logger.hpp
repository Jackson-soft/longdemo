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

namespace uranus::log {
// Logger 日志的主类
class Logger : public utils::Noncopyable {
public:
    Logger() = default;

    ~Logger() {
        auto *fpt = formatter_.release();
        delete fpt;
        auto *fbt = backend_.release();
        delete fbt;
    }

    //单例实例
    static auto Get() -> std::shared_ptr<Logger> {
        static auto logger = std::make_shared<Logger>();
        return logger;
    }

    //初始化，只需要调一次
    auto Set(const LogLevel &lvl, std::unique_ptr<Formatter> fmt, std::unique_ptr<Backend> backend) -> bool {
        std::lock_guard<std::mutex> locker(mutex_);
        level_ = lvl;
        formatter_.swap(fmt);
        backend.swap(backend);
        return true;
    }

    //日志输出对外接口
    void InfoLn(std::string_view msg) {
        return outPut(LogLevel::INFO, msg);
    }

    void Run() {
        while (true) {
            if (!buffer_.empty()) {
                backend_->Write(buffer_.front());
                buffer_.pop();
            }
        }
    }

private:
    //输出
    void outPut(LogLevel lvl, std::string_view msg) {
        std::lock_guard<std::mutex> locker(mutex_);
        if (lvl >= level_) {
            buffer_.push(formatter_->Format(msg));
        }
    }

    std::mutex                 mutex_;
    LogLevel                   level_;
    std::unique_ptr<Formatter> formatter_;  //格式化前端
    std::unique_ptr<Backend>   backend_;    //输出后端
    std::queue<std::string>    buffer_;     //环型缓存
};

#define LOG_INFO Logger::Get()::InfoLn()
}  // namespace uranus::log
