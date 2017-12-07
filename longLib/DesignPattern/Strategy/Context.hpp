//策略模式的环境类
#pragma once

#include "Strategy.hpp"

class Context
{
public:
	Context() {}
	virtual ~Context() = default;

	void Algorithm() { mStrategy->Algorithm(); }
	void SetStrategy(Strategy *st) { mStrategy = st; }

private:
	Strategy *mStrategy;
};
