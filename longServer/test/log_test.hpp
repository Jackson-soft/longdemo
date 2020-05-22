#pragma once

#include "doctest.h"
#include "log/log_level.hpp"
#include "log/logger.hpp"
#include <string>

TEST_CASE("test log")
{
    SUBCASE("test unmarshal log level")
    {
        std::string lvl = "infor";
        auto ll         = Uranus::Log::Unmarshal(lvl);
        CHECK(ll == Uranus::Log::Level::INFOR);
    }

    SUBCASE("test marshal log level")
    {
        auto ll  = Uranus::Log::Level::INFOR;
        auto lvl = Uranus::Log::Marshal(ll);
        CHECK(lvl == "infor");
    }
}