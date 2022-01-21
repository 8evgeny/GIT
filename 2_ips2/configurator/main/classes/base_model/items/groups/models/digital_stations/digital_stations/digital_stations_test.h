#pragma once
#include <catch.hpp>
#include "group_digital_stations.h"

TEST_CASE("Test case DigitalStation class") {
    SECTION("Create a default DigitalStation") {
        DigitalStation d;
        REQUIRE(d.nameDigitalStation() == IPS_STR("DigitalStation"));
        REQUIRE(d.id() == 0);
    }
    SECTION("Create a new Values DigitalStation") {
        DigitalStation d = {IPS_STR("TestDigitalStation"), 1};
        REQUIRE(d.nameDigitalStation() == IPS_STR("TestDigitalStation"));
        REQUIRE(d.id() == 1);
    }
}
