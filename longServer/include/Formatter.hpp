#pragma once

#include "LogLevel.hpp"
#include "TimeUtil.hpp"
#include <boost/format.hpp>
#include <cstdio>
#include <map>
#include <string>
#include <string_view>

// Formatter 日志格式化前端
class Formatter
{
public:
	Formatter() {}
	virtual ~Formatter() {}

    virtual std::string Format(Level lvl, std::string_view msg)		   = 0;
	virtual void WithFields(std::map<std::string, std::string> fields) = 0;

private:
	std::map<std::string, std::string> mData;
};

class TextFormatter : public Formatter
{
public:
	TextFormatter() {}
	~TextFormatter() override {}

    std::string Format(Level lvl, std::string_view msg) override
	{
		std::string retData{""};
        std::sprintf(retData.data(),
                     "%s [%s] %s:%d :: ",
                     TimeUtil::GetCurrentDay().data(),
                     lvl.String().data(),
                     __FILE__,
                     __LINE__);
        if (mData.size() > 0) {
			for (const auto &i : mData) {
                std::sprintf(retData.data(),
                             "%s%s:%s,",
                             retData.data(),
                             i.first.data(),
                             i.second.data());
			}
		}
        std::sprintf(retData.data(), "%s%s\n", retData.data(), msg.data());
        return retData;
	}

	void WithFields(std::map<std::string, std::string> fields) override
	{
		mData.insert(fields.begin(), fields.end());
	}

private:
	std::map<std::string, std::string> mData;
};
