#pragma once

#include "Socket.hpp"
#include "doctest.h"

TEST_CASE("socket test")
{
    Uranus::Socket fd;
    fd.NewSocket();
    CHECK(fd.GetNativeFD() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}