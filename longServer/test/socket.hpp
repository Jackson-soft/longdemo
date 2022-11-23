#pragma once

#include "net/tcp_socket.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("socket test") {
    uranus::net::TCPConn fd;
    fd.Open();
    CHECK(fd.Native() > 0);
    CHECK(fd.Listen());
    CHECK(fd.Bind(5005));
}