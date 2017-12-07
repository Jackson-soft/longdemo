#pragma once

// 具体目标
#include "Subject.hpp"

#include <vector>

class Observer;

class ConcreteSubject : public Subject
{
public:
	ConcreteSubject()  = default;
	~ConcreteSubject() = default;

	void Attach(Observer *pObserver);
	void Detach(Observer *pObserver);

	void Notify();

private:
	std::vector<Observer *> mObservers;
};

void ConcreteSubject::Attach(Observer *pObserver)
{
	mObservers.emplace_back(pObserver);
}

void ConcreteSubject::Detach(Observer *pObserver)
{
	for (auto it : mObservers) {
		if (it == pObserver) {
			mObservers.erase(pObserver);
			return;
		}
	}
}

void ConcreteSubject::Notify()
{
	for (auto it : mObservers) {
		it->Update();
	}
}