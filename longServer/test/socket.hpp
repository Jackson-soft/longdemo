#pragma once

#include "doctest.h"
#include "net/socket.hpp"

TEST_CASE("socket test")
{
    uranus::net::Socket fd;
    fd.NewSocket();
    CHECK(fd.GetNativeFD() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}