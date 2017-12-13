#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_MODULE time_test

#include "../TimeUtil.hpp"
#include <boost/test/included/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(time_test)

BOOST_AUTO_TEST_CASE(current_time)
{
	std::cout << TimeUtil::GetCurrentTime() << std::endl;
}

BOOST_AUTO_TEST_CASE(current_day)
{
	std::cout << TimeUtil::GetCurrentDay() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
