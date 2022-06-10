#pragma once
#include <catch.hpp>
#include "list_stations.h"
#include "classes/base_model/items/table_models/model_table_groups/model_table_digital_stations/group_digital_stations/group_digital_stations.h"

TEST_CASE("Test case GroupCall class") {
    SECTION("Create a default GroupCall") {
        GroupCall g;
        REQUIRE(g.nameGroupCall() == IPS_STR("NameGroupCall"));
        REQUIRE(g.id() == 0);
        REQUIRE(g.station().isEmpty());
    }
    SECTION("Create a GroupCall") {
        QList<DigitalStation> list = {{IPS_STR("TestDigitalStation"), 1}};
        GroupCall g = {IPS_STR("NameGroupCall"), 1, list};
        REQUIRE(g.nameGroupCall() == IPS_STR("NameGroupCall"));
        REQUIRE(g.id() == 1);
        REQUIRE_FALSE(g.id() == 0);
        REQUIRE(g.station().value(0).nameDigitalStation() == IPS_STR("TestDigitalStation"));
        REQUIRE(g.station().value(0).id() == 1);
    };
}
