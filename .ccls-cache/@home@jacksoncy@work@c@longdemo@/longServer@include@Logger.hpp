#pragma once

#include "Backend.hpp"
#include "Formatter.hpp"
#include "LogLevel.hpp"
#include "Util.hpp"
#include <mutex>
#include <queue>
#include <string>
#include <string_view>

// Logger 日志的主类
class Logger : public Noncopyable
{
public:
	Logger() = default;

	~Logger()
	{
		delete mFormatter;
		delete mBackend;
	}

	//单例实例
	static Logger *GetInstance()
	{
		static Logger logger;
		return &logger;
	}

	//初始化，只需要调一次
	bool Initialization(LogLevel level, Formatter *fmt, Backend *backend)
	{
		mMutex.lock();
		mLevel	 = level;
		mFormatter = fmt;
		mBackend   = backend;
		mMutex.unlock();
		return true;
	}

	//日志输出对外接口void Inforln(std::string_view msg) { return outPut(LogLevel::INFOR, msg); }

	void Run()
	{
		while (true) {
			if (mBuffer.Size() > 0) {
				mBackend->Write(mBuffer.front());
				mBuffer.pop();
			}
		}
	}

private:
	//输出
	void outPut(LogLevel level, std::string_view msg)
	{
		if (level >= mLevel) {
			mBuffer.push(mFormatter->Format(msg));
		}
	}

private:
	std::mutex mMutex;
	LogLevel mLevel;
	Formatter *mFormatter;			 //格式化前端
	Backend *mBackend;				 //输出后端
	std::queue<std::string> mBuffer; //环型缓存
};

#define LOG_INFO Logger::GetInstance().Inforln()
