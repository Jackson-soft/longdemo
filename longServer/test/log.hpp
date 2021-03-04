#pragma once

#include "doctest.h"
#include "log/Logger.hpp"
#include "log/level.hpp"
#include <string>

TEST_CASE("test log")
{
    SUBCASE("test unmarshal log level")
    {
        std::string lvl = "infor";
        auto ll         = uranus::log::Level::Unmarshal(lvl);
        CHECK(ll == uranus::log::logLevel::INFOR);
    }

    SUBCASE("test marshal log level")
    {
        auto ll  = uranus::log::logLevel::INFOR;
        auto lvl = uranus::log::Level::Marshal(ll);
        CHECK(lvl == "infor");
    }
}