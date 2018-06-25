#pragma once

#include <map>
#include <string>

// Formatter 日志格式化前端
class Formatter
{
public:
	Formatter() {}
	virtual ~Formatter() {}

	virtual std::string Format()									   = 0;
	virtual void WithFields(std::map<std::string, std::string> fields) = 0;

private:
	std::map<std::string, std::string> mData;
};

class TextFormatter : public Formatter
{
public:
	TextFormatter() {}
	~TextFormatter() override {}

	std::string Format() override { return ""; }

	void WithFields(std::map<std::string, std::string> fields) override {}

private:
	std::map<std::string, std::string> mData;
};