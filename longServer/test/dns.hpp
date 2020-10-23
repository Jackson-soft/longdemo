#pragma once

#include "doctest.h"
#include "net/dns.hpp"
#include <iostream>

TEST_CASE("dns test")
{
    Uranus::Net::DNS dns;
    CHECK(dns.Resolve("www.baidu.com") == true);

    for (auto &it : dns.LookupIP()) {
        std::cout << it << std::endl;
    }
}