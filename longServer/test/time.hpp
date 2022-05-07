#pragma once

#include "utils/time.hpp"

#include <catch2/catch.hpp>

TEST_CASE("time test") {
    INFO("current day is " << uranus::utils::CurrentDay());
    INFO("current time is " << uranus::utils::CurrentTime());
    INFO("mil unix time is " << uranus::utils::MilUnixTime());
    INFO("unix time is " << uranus::utils::UnixTime());
}
