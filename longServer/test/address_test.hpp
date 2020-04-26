#pragma once

#include "doctest.h"
#include "net/address.hpp"

TEST_CASE("test address")
{
    SUBCASE("test address v6")
    {
        Uranus::Net::IPv6Addr addr;
        addr.Resolve("[2111:500:4:13::128]:443");
        MESSAGE(addr.String());
        addr.Resolve("[::1]:443");
        MESSAGE(addr.String());
        CHECK(addr.isLoop() == true);
    }

    SUBCASE("test address zone")
    {
        Uranus::Net::IPv6Addr addr;
        addr.Resolve("[2111:500:4:13::128%3344]:443");
        MESSAGE(addr.String());
        CHECK(addr.isLoop() == false);
    }
}
