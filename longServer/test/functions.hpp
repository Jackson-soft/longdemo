#pragma once

#include <catch2/catch.hpp>
#include <utility>
#include <vector>

// test something
TEST_CASE("test remove") {
    std::vector<int> a{2, 34, 5, 6, 8};
    a.at(2) = a.back();
    a.pop_back();

    CHECK(a.size() == 4);
}