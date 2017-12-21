#include <iostream>

#include "../../longServer/src/TimeUtility.h"

#include <string>

int main()
{
	std::string a = TimeUtility::GetCurrentDay();
	std::cout << a << std::endl;
	std::string b = TimeUtility::GetCurrentTime();
	std::cout << b << std::endl;
	return 0;
}
