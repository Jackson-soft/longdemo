#pragma once

#include "log/level.hpp"
#include "log/logger.hpp"

#include <catch2/catch.hpp>
#include <string>

TEST_CASE("test log") {
    SECTION("test unmarshal log level") {
        std::string lvl = "info";
        auto        ll  = uranus::log::Level::Unmarshal(lvl);
        CHECK(ll == uranus::log::LogLevel::INFO);
    }

    SECTION("test marshal log level") {
        auto ll  = uranus::log::LogLevel::INFO;
        auto lvl = uranus::log::Level::Marshal(ll);
        CHECK(lvl == "infor");
    }
}