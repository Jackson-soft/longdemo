//抽象策略类
#pragma once

class Strategy
{
public:
	Strategy();
	virtual ~Strategy();

	virtual void Algorithm() = 0;
};
