//具体策略类
#pragma once

#include "Strategy.hpp"

class ConcreteStrategy : public Strategy
{
public:
	ConcreteStrategy();
	virtual ~ConcreteStrategy();

	virtual void algorithm();
};
