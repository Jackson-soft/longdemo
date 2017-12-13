// 目标类

#pragma once

#include "Observer.hpp"
#include <map>
#include <string>

template <typename Func>
class Subject
{
public:
	Subject()		   = default;
	virtual ~Subject() = default;

    // 订阅
	virtual void Attach(Observer *pObserver) = 0;

    // 取消
	virtual void Detach(Observer *pObserver) = 0;

    // 通知
    virtual void Notify(const std::string &msg) = 0;

	// virtual int GetState()  = 0;
	// virtual void SetState() = 0;
};
