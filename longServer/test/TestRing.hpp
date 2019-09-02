#pragma once

#include "RingBuffer.hpp"
#include "doctest.h"
#include <vector>

TEST_CASE("dd")
{
    std::vector<int> v(5);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
}
