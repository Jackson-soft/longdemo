#pragma once

#include "doctest.h"
#include "utils/time_util.hpp"

TEST_CASE("time test")
{
    MESSAGE("current day is " << Uranus::Utils::TimeUtil::CurrentDay());
    MESSAGE("current time is " << Uranus::Utils::TimeUtil::CurrentTime());
    MESSAGE("mil unix time is " << Uranus::Utils::TimeUtil::MilUnixTime());
    MESSAGE("unix time is " << Uranus::Utils::TimeUtil::UnixTime());
}