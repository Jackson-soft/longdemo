#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <string>

// Backend 日志输出后端
class Backend
{
public:
	Backend() {}
	virtual ~Backend() {}

	virtual void Write() = 0;
	virtual void Close() = 0;
};

class FileBackend : public Backend
{
public:
	FileBackend() {}
    ~FileBackend() override {}

	void Write() override {}

	void Close() override { mFile.close(); }

private:
	std::fstream mFile;					 //文件流
	std::string mPath{""};				 //存放日志文件的目录
	std::string mLink{""};				 //当前写入的日志文件链接
	std::string mPrefix{"zlog"};		 //日志名称前缀
	int64_t mMaxSize{500 * 1024 * 1024}; //日志文件大小限制
	std::string mAppellation{""};		 //日志文件的名称
	int mIndex{1};						 //日志文件序号
	std::string mCurrentDay{""};		 //当前日期
	bool mChang{false};					 //日志文件是否需要切割
};
