#pragma once
//具体观察者
#include "Observer.hpp"
#include <iostream>

class ConcreteObeserver : public Observer
{
public:
	ConcreteObeserver(std::string name);
	virtual ~ConcreteObeserver();

	virtual void Update(const std::string &msg);

	std::string GetName();

private:
	std::string mName;
    // int mObserverState;
};

ConcreteObeserver::ConcreteObeserver(std::string name) : mName(name) {}

ConcreteObeserver::~ConcreteObeserver() {}

void ConcreteObeserver::Update(const std::string &msg)
{
    std::cout << "this is " << mName.data() << " : " << msg.data() << std::endl;
}

std::string ConcreteObeserver::GetName() { return mName; }
