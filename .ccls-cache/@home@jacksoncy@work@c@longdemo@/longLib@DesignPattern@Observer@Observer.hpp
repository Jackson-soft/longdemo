// 观察者类
#pragma once

#include <string>

class Observer
{
public:
	Observer()			= default;
	virtual ~Observer() = default;

	virtual void Update(const std::string &msg) = 0;

	virtual std::string GetName() = 0;
};
