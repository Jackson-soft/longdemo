#pragma once

// 具体目标
#include "Subject.hpp"

#include <list>

class Observer;

class ConcreteSubject : public Subject
{
public:
	ConcreteSubject()  = default;
	~ConcreteSubject() = default;

	void Attach(Observer *pObserver);
	void Detach(Observer *pObserver);

    void Notify(const std::string &msg);

private:
    std::list<Observer *> mObservers;
};

void ConcreteSubject::Attach(Observer *pObserver)
{
	mObservers.emplace_back(pObserver);
}

void ConcreteSubject::Detach(Observer *pObserver)
{
    for (auto it = mObservers.begin(); it != mObservers.end();) {
        if (*it == pObserver) {
            it = mObservers.erase(it);
        } else {
            ++it;
        }
    }
}

void ConcreteSubject::Notify(const std::string &msg)
{
	for (auto it : mObservers) {
        it->Update(msg);
	}
}
