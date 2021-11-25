#pragma once

#include "log/level.hpp"
#include "log/logger.hpp"

#include <doctest/doctest.h>
#include <string>

TEST_CASE("test log")
{
    SUBCASE("test unmarshal log level")
    {
        std::string lvl = "info";
        auto        ll  = uranus::log::Level::Unmarshal(lvl);
        CHECK(ll == uranus::log::LogLevel::INFO);
    }

    SUBCASE("test marshal log level")
    {
        auto ll  = uranus::log::LogLevel::INFO;
        auto lvl = uranus::log::Level::Marshal(ll);
        CHECK(lvl == "infor");
    }
}