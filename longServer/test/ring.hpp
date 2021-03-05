#pragma once

#include "doctest/doctest.h"
#include "utils/ring_buffer.hpp"
#include <vector>

TEST_CASE("dd")
{
    std::vector<int> v(5);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
}
