#include <iostream>

#include "ConcreteStrategy.h"
#include "Context.h"
#include "Strategy.hpp"

int main()
{
	Strategy *sy = new ConcreteStrategy();
	Context *ct  = new Context();

	ct->setStrategy(sy);
	ct->algorithm();

	delete sy;
	delete ct;
	return 0;
}
