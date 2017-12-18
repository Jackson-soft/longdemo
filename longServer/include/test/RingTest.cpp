#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include "../RingBuffer.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(ringTest)

BOOST_AUTO_TEST_CASE(ringPush)
{
	std::cout << "start test" << std::endl;
	int n = 5;
	RingBuffer<int> nRing(n);
	// RingBuffer<int> nRing;
	for (int i = 0; i < 6; ++i) {
		nRing.Write(i);
	}

	for (int i = 0; i < n; ++i) {
		std::cout << nRing.Read() << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()