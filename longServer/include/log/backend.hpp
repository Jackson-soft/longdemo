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

namespace uranus::log
{
// Backend 日志输出后端接口
class Backend
{
public:
    Backend()          = default;
    virtual ~Backend() = default;

    virtual void Write(std::string_view buf) = 0;
    virtual void Close()                     = 0;
};

// 终端输出后端
class ConsoleBackend: public Backend
{
};

// 文件输出后端
class FileBackend: public Backend
{
public:
    FileBackend() = default;
    ~FileBackend() override { Close(); }

    //初始化各项参数
    bool InitBackend(std::string_view path,
                     int64_t maxSize           = 0,
                     std::string_view prefix   = "",
                     std::string_view fileLink = "")
    {
        if (path.size() <= 0) {
            return false;
        } else {
            mPath = path;
        }

        if (prefix.size() > 0) {
            mPrefix = prefix;
        }
        if (fileLink.size() > 0) {
            mLink = fileLink;
        }

        if (maxSize > 0) {
            mMaxSize = maxSize * 1024 * 1024;
        }

        if (!std::filesystem::exists(mPath)) {
            if (!std::filesystem::create_directory(mPath)) {
                return false;
            }
        }

        mCurrentDay = utils::CurrentDay();

        if (mChang) {
            createFile();
        }

        return true;
    }

    void Write(std::string_view buf) override
    {
        doIncise();
        mFile.write(buf.data(), buf.size());
    }

    void Close() override
    {
        mFile.flush();
        mFile.close();
        mFile.clear();
    }

private:
    //文件切割逻辑
    void doIncise()
    {
        checkData();
        checkSize();
        if (mChang) {
            createFile();
        }
    }

    //检查日期
    void checkData()
    {
        auto tDay = utils::CurrentDay();
        if (tDay != mCurrentDay) {
            mCurrentDay = tDay;
            mIndex      = 1;
            mChang      = true;
        }
    }

    //检查文件
    void checkSize()
    {
        auto nFileSize = std::filesystem::file_size(mAppellation);
        if (nFileSize > mMaxSize) {
            mIndex++;
            mChang = true;
        }
    }

    //打开文件
    void createFile()
    {
        mAppellation = fmt::format("{}/{}-{}-{:4d}.log", mPath, mPrefix, mCurrentDay, mIndex);

        mFile.open(mAppellation, std::ios::in);
        if (mLink.size() > 0) {
            //创建文件软链接
        }
        mChang = false;
    }

private:
    std::fstream mFile;                         //文件流
    std::string mPath{""};                      //存放日志文件的目录
    std::string mLink{""};                      //当前写入的日志文件链接
    std::string mPrefix{"zlog"};                //日志名称前缀
    std::uint64_t mMaxSize{500 * 1024 * 1024};  //日志文件大小限制
    std::string mAppellation{""};               //日志文件的名称
    std::uint32_t mIndex{1};                    //日志文件序号
    std::string mCurrentDay{""};                //当前日期
    bool mChang{true};                          //日志文件是否需要切割
};
}  // namespace uranus::log