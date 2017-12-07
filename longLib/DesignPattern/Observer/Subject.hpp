// 目标类

#pragma once

#include "Observer.hpp"

class Subject
{
public:
	Subject()		   = default;
	virtual ~Subject() = default;

	virtual void Attach(Observer *pObserver) = 0;
	virtual void Detach(Observer *pObserver) = 0;

	virtual void Notify() = 0;

	// virtual int GetState()  = 0;
	// virtual void SetState() = 0;
};
