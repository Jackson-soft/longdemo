//策略模式的环境类
#pragma once

#include "Strategy.hpp"

class Context
{
public:
	Context();
	virtual ~Context();

	void algorithm();
	void setStrategy(Strategy *st);

private:
	Strategy *m_pStrategy;
};
