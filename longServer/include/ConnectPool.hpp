#pragma once
// 连接池

#include "Util.hpp"
#include <mutex>
#include <string_view>

class ConnectPool : public Noncopyable
{
public:
	static ConnectPool *GetInstance()
	{
		static ConnectPool connPool;
		return &connPool;
	}

public:
	ConnectPool() {}

	~ConnectPool() {}

	void Initalize(std::string_view conn, int maxConn = 0, int maxIdle = 0)
	{
		mConnect = conn;
		mMaxIdle = maxIdle;
		mMaxConn = maxConn;
	}

private:
	std::mutex mMutex;
	std::string_view mConnect; // 连接
	int mMaxIdle{0};		   // 最大空闲数
	int mMaxConn{0};		   // 最大连接数
};
