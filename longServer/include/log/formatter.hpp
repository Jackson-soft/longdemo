#pragma once

#include "utils/time.hpp"

#include <cstdio>
#include <fmt/format.h>
#include <map>
#include <string>
#include <string_view>

namespace uranus::log {
// Formatter 日志格式化前端接口
class Formatter {
public:
    Formatter()                                                         = default;
    virtual ~Formatter()                                                = default;

    virtual auto Format(std::string_view msg) -> std::string            = 0;
    virtual void WithFields(std::map<std::string, std::string> &fields) = 0;
};

// 文件格式化前端
class TextFormatter : public Formatter {
public:
    TextFormatter() : TextFormatter("infor") {}

    explicit TextFormatter(std::string_view lvl) : level_{lvl} {}

    ~TextFormatter() override {
        data_.clear();
    }

    auto Format(std::string_view msg) -> std::string override {
        std::string retData{};
        std::sprintf(retData.data(),
                     "%s [%s] %s:%d :: ",
                     utils::CurrentDay().data(),
                     level_.data(),
                     __FILE__,
                     __LINE__);
        if (!data_.empty()) {
            for (const auto &i : data_) {
                std::sprintf(retData.data(), "%s%s:%s,", retData.data(), i.first.data(), i.second.data());
            }
        }
        std::sprintf(retData.data(), "%s%s\n", retData.data(), msg.data());
        return retData;
    }

    void WithFields(std::map<std::string, std::string> &fields) override {
        data_.insert(fields.begin(), fields.end());
    }

private:
    std::map<std::string, std::string> data_;   // 附加信息
    std::string                        level_;  // 日志级别
};
}  // namespace uranus::log
