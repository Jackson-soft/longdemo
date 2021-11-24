#pragma once

#include <doctest/doctest.h>
#include <utility>
#include <vector>

// test something
TEST_CASE("test remove")
{
    std::vector<int> a{2, 34, 5, 6, 8};
    a.at(2) = std::move(a.back());
    a.pop_back();

    CHECK(a.size() == 4);
}