#pragma once
#include <catch.hpp>
#include "classes/json.h"
#include "group_calls/group_calls.h"
#include "group_calls/list_stations.h"
#include "classes/base_model/items/table_models/model_table_groups/model_table_groups_calls/model_table_groups_calls.h"

TEST_CASE("Test case tablegroupscalls class") {
   ModelTableGroupsCalls m;
   REQUIRE(m.columnCount() == 2);
   REQUIRE(m.insertRows(0, 1));
   REQUIRE(m.rowCount() == 1);

   SECTION("Check serialization") {
        auto const& s = m.serialize();
        REQUIRE_FALSE(s.isEmpty());
        auto const& f = s.first().toObject();
        REQUIRE(json::getString(f, IPS_STR("name"), IPS_STR("NameGroupCall")) == IPS_STR("NameGroupCall"));
        REQUIRE(json::getString(f, IPS_STR("id"), IPS_STR("")) == QString());
        REQUIRE(json::getString(f, IPS_STR("stations"), IPS_STR("")) == QString());
   }
   SECTION("Check ClearAllStations") {
        REQUIRE(m.ClearAllStations(0));
        REQUIRE_FALSE(m.ClearAllStations(-1));
   }
   QJsonArray arr;
   QJsonObject json = {
             {IPS_STR("name"), IPS_STR("NameGroupCall")}
           , {IPS_STR("id"),  0}
   };
   arr.push_back(json);
   QJsonArray arrEmpty;
   QJsonObject jsonEmpty = {};
   arrEmpty.push_back(jsonEmpty);

   SECTION("Check deserialization") {
        REQUIRE(m.deserialize(arr));
        REQUIRE(m.deserialize(arrEmpty));
   }
}
