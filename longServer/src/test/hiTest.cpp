#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_MODULE test_hello

#include <boost/test/included/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(test_hello)

BOOST_AUTO_TEST_CASE(free_test_function) { BOOST_TEST(true); }

BOOST_AUTO_TEST_CASE(hello) { BOOST_CHECK(1 == 1); }

BOOST_AUTO_TEST_CASE(hello1) { std::cout << "ddd" << std::endl; }

BOOST_AUTO_TEST_SUITE_END()
