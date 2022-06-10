#pragma once
#include <catch.hpp>
#include "classes/types.h"

TEST_CASE("Test case station_type_enums class") {
    SECTION("Create test for toString function") {
        REQUIRE(toString(StationType::Invalid) == IPS_STR("invalid"));
        REQUIRE(toString(StationType::Unknown) == IPS_STR("unknown"));
        REQUIRE(toString(StationType::UM120) == IPS_STR("UM120"));
        REQUIRE(toString(StationType::UM240) == IPS_STR("UM240"));
        REQUIRE(toString(StationType::PDO32) == IPS_STR("PDO32"));
        REQUIRE(toString(StationType::PDO16) == IPS_STR("PDO16"));
        REQUIRE(toString(StationType::PDO8) == IPS_STR("PDO8"));
        REQUIRE(toString(StationType::UPW2) == IPS_STR("UPW2"));
        REQUIRE(toString(StationType::UPW4) == IPS_STR("UPW4"));
        REQUIRE(toString(StationType::UPW6) == IPS_STR("UPW6"));
    }
    SECTION("Create test to jumble for toString function") {
        REQUIRE(toString(StationType::Invalid) != IPS_STR("UPW4"));
        REQUIRE(toString(StationType::Unknown) != IPS_STR("UM120"));
        REQUIRE(toString(StationType::UPW4) != IPS_STR("PDO16"));
        REQUIRE(toString(StationType::UPW6) != IPS_STR("unknown"));
    }
    SECTION("Create test for toNumber function") {
        REQUIRE(toNumber(StationType::Invalid) == 0);
        REQUIRE(toNumber(StationType::Unknown) == 0);
        REQUIRE(toNumber(StationType::UM120) == 0);
        REQUIRE(toNumber(StationType::UM240) == 0);
        REQUIRE(toNumber(StationType::PDO32) == 32);
        REQUIRE(toNumber(StationType::PDO16) == 16);
        REQUIRE(toNumber(StationType::PDO8) == 8);
        REQUIRE(toNumber(StationType::UPW2) == 2);
        REQUIRE(toNumber(StationType::UPW4) == 4);
        REQUIRE(toNumber(StationType::UPW6) == 6);
    }
    SECTION("Create test to jumble for toNumber function") {
        REQUIRE(toNumber(StationType::Invalid) != 120);
        REQUIRE(toNumber(StationType::Unknown) != 16);
        REQUIRE(toNumber(StationType::UPW4) != 8);
        REQUIRE(toNumber(StationType::UPW6) != 4);
    }
}
