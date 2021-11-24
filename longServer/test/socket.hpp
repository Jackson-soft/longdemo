#pragma once

#include "net/socket.hpp"

#include <doctest/doctest.h>

TEST_CASE("socket test")
{
    uranus::net::Socket fd;
    fd.newSocket();
    CHECK(fd.getNativeFD() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}