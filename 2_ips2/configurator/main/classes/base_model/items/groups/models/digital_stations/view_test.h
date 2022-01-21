#pragma once
#include <catch.hpp>
#include "classes/json.h"
#include "classes/base_model/items/table_models/model_table_groups/model_table_digital_stations/model_table_view.h"

TEST_CASE("Test case modeltableview class") {
   ModelTableView m;
   REQUIRE(m.columnCount() == 2);
   REQUIRE(m.insertRows(0, 1));
   REQUIRE(m.rowCount() == 1);

   SECTION("Check serialization") {
        auto const& s = m.serialize();
        REQUIRE_FALSE(s.isEmpty());
        auto const& f = s.first().toObject();
        REQUIRE(json::getString(f, IPS_STR("name"), IPS_STR("DigitalStation")) == IPS_STR("DigitalStation"));
        REQUIRE(json::getString(f, IPS_STR("id"), IPS_STR("")) == QString());
   }
   SECTION("Check setStation") {
        DigitalStation station;
        REQUIRE(m.setStation(0, station));
        REQUIRE_FALSE(m.setStation(-1, station));
   }
   QJsonArray arr;
   QJsonObject json = {
        {IPS_STR("name"), IPS_STR("DigitalStation")}
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
