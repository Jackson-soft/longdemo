#pragma once

#include "doctest.h"
#include "net/Socket.hpp"

TEST_CASE("socket test")
{
    Uranus::Net::Socket fd;
    fd.NewSocket();
    CHECK(fd.GetNativeFD() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}