#pragma once

#include "doctest.h"
#include "net/socket.hpp"

TEST_CASE("socket test")
{
    Uranus::Net::Socket fd;
    fd.NewSocket();
    CHECK(fd.NativeFD() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}
