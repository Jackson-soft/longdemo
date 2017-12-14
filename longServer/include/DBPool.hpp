#pragma once
// 数据库连接池

#include "Util.hpp"
#include <mutex>

class DBPool : public Noncopyable
{
public:
	DBPool() : DBPool(0, 0) {}

	DBPool(int maxIdle, int maxConn) {}

	~DBPool() {}

private:
	std::mutex mMutex;
	int mMaxIdle; //最大空闲数
	int mMaxConn; // 最大连接数
};