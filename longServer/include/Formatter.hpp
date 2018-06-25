#pragma once

#include "LogLevel.hpp"
#include "TimeUtil.hpp"
#include <boost/format.hpp>
#include <map>
#include <string>
#include <string_view>

// Formatter 日志格式化前端
class Formatter
{
public:
	Formatter() {}
	virtual ~Formatter() {}

	virtual std::string Format(LogLevel lvl, std::string_view msg)	 = 0;
	virtual void WithFields(std::map<std::string, std::string> fields) = 0;

private:
	std::map<std::string, std::string> mData;
};

class TextFormatter : public Formatter
{
public:
	TextFormatter() {}
	~TextFormatter() override {}

    std::string Format(LogLevel lvl, std::string_view msg) override
    {
        return boost::str(boost::format("%s [%s] %s:%d ::") %
                          TimeUtil::GetCurrentDay().c_str() % lvl.String() %
                          __FILE__ % __LINE__);
    }

	void WithFields(std::map<std::string, std::string> fields) override {}

private:
	std::map<std::string, std::string> mData;
};
