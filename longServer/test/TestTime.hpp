#pragma once

#include "TimeUtil.hpp"
#include "doctest.h"

TEST_CASE("time test")
{
    MESSAGE("current day is " << Uranus::TimeUtil::CurrentDay());
    MESSAGE("current time is " << Uranus::TimeUtil::CurrentTime());
    MESSAGE("mil unix time is " << Uranus::TimeUtil::MilUnixTime());
    MESSAGE("unix time is " << Uranus::TimeUtil::UnixTime());
}