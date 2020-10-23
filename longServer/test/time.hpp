#pragma once

#include "doctest.h"
#include "utils/time.hpp"

TEST_CASE("time test")
{
    MESSAGE("current day is " << Uranus::Utils::CurrentDay());
    MESSAGE("current time is " << Uranus::Utils::CurrentTime());
    MESSAGE("mil unix time is " << Uranus::Utils::MilUnixTime());
    MESSAGE("unix time is " << Uranus::Utils::UnixTime());
}