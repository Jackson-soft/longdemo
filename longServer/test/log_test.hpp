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
        auto ll         = Uranus::Log::Level::Unmarshal(lvl);
        CHECK(ll == Uranus::Log::LogLevel::INFOR);
    }

    SUBCASE("test marshal log level")
    {
        auto ll  = Uranus::Log::LogLevel::INFOR;
        auto lvl = Uranus::Log::Level::Marshal(ll);
        CHECK(lvl == "infor");
    }
}