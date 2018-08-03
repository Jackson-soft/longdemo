#pragma once

#include "TimeUtil.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

// Backend 日志输出后端
class Backend
{
public:
	Backend() {}
	virtual ~Backend() {}

    virtual void Write(std::string_view buf) = 0;
    virtual void Close()					 = 0;
};

class FileBackend : public Backend
{
public:
	FileBackend() {}
	~FileBackend() override { Close(); }

    //初始化各项参数
    bool InitBackend(std::string_view path,
                     int64_t maxSize		   = 0,
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

        if (!std::filesystem::exists(path)) {
            if (!std::filesystem::create_directory(path)) {
                return false;
            }
        }

        mCurrentDay = TimeUtil::GetCurrentDay();

        mFile.open("xxx", std::ios::in);

        return true;
    }

    void Write(std::string_view buf) override
    {
        doIncise();
        mFile.write(buf.data(), 10);
    }

	void Close() override
	{
		mFile.close();
		mFile.clear();
	}

private:
	//文件切割逻辑
	void doIncise() {}

	//检查日期
	void checkData() {}

	//检查文件
	void checkSize() {}

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
