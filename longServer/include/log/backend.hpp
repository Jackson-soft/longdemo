#pragma once

#include "utils/time.hpp"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <string>
#include <string_view>

namespace uranus::log {
// Backend 日志输出后端接口
class Backend {
public:
    Backend()                                = default;
    virtual ~Backend()                       = default;

    virtual void Write(std::string_view buf) = 0;
    virtual void Close()                     = 0;
};

// 终端输出后端
class ConsoleBackend : public Backend {};

// 文件输出后端
class FileBackend : public Backend {
public:
    FileBackend() = default;
    ~FileBackend() override {
        Close();
    }

    //初始化各项参数
    auto InitBackend(std::string_view path,
                     int64_t          maxSize  = 0,
                     std::string_view prefix   = "",
                     std::string_view fileLink = "") -> bool {
        if (path.empty()) {
            return false;
        }
        path_ = path;

        if (!prefix.empty()) {
            prefix_ = prefix;
        }
        if (!fileLink.empty()) {
            link_ = fileLink;
        }

        if (maxSize > 0) {
            maxSize_ = maxSize * 1024 * 1024;
        }

        if (!std::filesystem::exists(path_)) {
            if (!std::filesystem::create_directory(path_)) {
                return false;
            }
        }

        currentDay_ = utils::CurrentDay();

        if (chang_) {
            createFile();
        }

        return true;
    }

    void Write(std::string_view buf) override {
        doIncise();
        file_.write(buf.data(), buf.size());
    }

    void Close() override {
        file_.flush();
        file_.close();
        file_.clear();
    }

private:
    //文件切割逻辑
    void doIncise() {
        checkData();
        checkSize();
        if (chang_) {
            createFile();
        }
    }

    //检查日期
    void checkData() {
        auto tDay = utils::CurrentDay();
        if (tDay != currentDay_) {
            currentDay_ = tDay;
            index_      = 1;
            chang_      = true;
        }
    }

    //检查文件
    void checkSize() {
        auto nFileSize = std::filesystem::file_size(appellation_);
        if (nFileSize > maxSize_) {
            index_++;
            chang_ = true;
        }
    }

    //打开文件
    void createFile() {
        appellation_ = fmt::format("{}/{}-{}-{:4d}.log", path_, prefix_, currentDay_, index_);

        file_.open(appellation_, std::ios::in);
        if (!link_.empty()) {
            //创建文件软链接
        }
        chang_ = false;
    }

    std::fstream  file_;                        //文件流
    std::string   path_;                        //存放日志文件的目录
    std::string   link_;                        //当前写入的日志文件链接
    std::string   prefix_{"zlog"};              //日志名称前缀
    std::uint64_t maxSize_{500 * 1024 * 1024};  //日志文件大小限制
    std::string   appellation_;                 //日志文件的名称
    std::uint32_t index_{1};                    //日志文件序号
    std::string   currentDay_;                  //当前日期
    bool          chang_{true};                 //日志文件是否需要切割
};
}  // namespace uranus::log