#include "ConcreteStrategy.h"

#include <iostream>

ConcreteStrategy::ConcreteStrategy() {}

ConcreteStrategy::~ConcreteStrategy() {}

void ConcreteStrategy::algorithm()
{
	std::cout << "ConcreteStrategy" << std::endl;
}
