#pragma once

#include "utils/time_util.hpp"
#include <cstdio>
#include <fmt/format.h>
#include <map>
#include <string>
#include <string_view>

namespace Uranus
{
namespace Log
{
// Formatter 日志格式化前端接口
class Formatter
{
public:
    Formatter()          = default;
    virtual ~Formatter() = default;

    virtual std::string Format(const std::string_view msg)              = 0;
    virtual void WithFields(std::map<std::string, std::string> &fields) = 0;
};

// 文件格式化前端
class TextFormatter : public Formatter
{
public:
    TextFormatter() : TextFormatter("infor") {}
    ~TextFormatter() override { mData.clear(); }

    TextFormatter(const std::string_view lvl) : mLevel{lvl} {}

    std::string Format(const std::string_view msg) override
    {
        std::string retData{""};
        std::sprintf(retData.data(),
                     "%s [%s] %s:%d :: ",
                     Utils::TimeUtil::CurrentDay().data(),
                     mLevel.data(),
                     __FILE__,
                     __LINE__);
        if (!mData.empty()) {
            for (const auto &i : mData) {
                std::sprintf(retData.data(), "%s%s:%s,", retData.data(), i.first.data(), i.second.data());
            }
        }
        std::sprintf(retData.data(), "%s%s\n", retData.data(), msg.data());
        return retData;
    }

    void WithFields(std::map<std::string, std::string> &fields) override { mData.insert(fields.begin(), fields.end()); }

private:
    std::map<std::string, std::string> mData;  // 附加信息
    std::string mLevel;                        // 日志级别
};
}  // namespace Log
}  // namespace Uranus