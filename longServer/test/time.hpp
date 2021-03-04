#pragma once

#include "doctest.h"
#include "utils/time.hpp"

TEST_CASE("time test")
{
    MESSAGE("current day is " << uranus::utils::CurrentDay());
    MESSAGE("current time is " << uranus::utils::CurrentTime());
    MESSAGE("mil unix time is " << uranus::utils::MilUnixTime());
    MESSAGE("unix time is " << uranus::utils::UnixTime());
}
