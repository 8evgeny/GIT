#pragma once
#include <catch.hpp>
#include "classes/json.h"
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/test_classes.h"
#include "classes/base_model/items/table_models/model_table_type_stations/model_table_type_stations.h"

TEST_CASE("Test case ModelTableTypeStations1 class") {
    BaseModel model;
    model.insertItem(2, new StationItems(), QModelIndex());
    auto parentItem = model.StationItems();
    LOG_FATAL(parentItem, "parentItem cannot be NULL");
    parentItem->appendChild(new StationItem(parentItem));
    auto typeItem = dynamic_cast<StationItem*>(model.StationItems()->child(0));
    LOG_FATAL(parentItem, "typeItem cannot be NULL");
    TestStationItem testItem(typeItem);

    REQUIRE(testItem.insertRows(0, 1));
    REQUIRE_FALSE(testItem.insertRows(0, 3));
    REQUIRE(testItem.rowCount() == 1);

    QJsonArray jsonItems;
    for (auto const& var : testItem.itemsActivStations()) {
            QJsonObject jsonItem = {
                    {IPS_STR("name"), var.name()}
                  , {IPS_STR("function"), var.function()}
                  , {IPS_STR("destination"), var.destination()}
                  , {IPS_STR("priority"), QString::number(var.priority())}
                  , {IPS_STR("pairs_stations"), var.serialize()}
            };
             jsonItems.push_back(jsonItem);
    }
    SECTION("Check serialize") {
            auto const& s = testItem.serializeModel();
            LOG_ERROR(s.isUndefined(), "s not found or invalid");
            REQUIRE_FALSE(s.isUndefined());
            auto const& f = s.toArray().first();
            LOG_ERROR(f.isUndefined(), "f not found or invalid");

            REQUIRE(json::getString(f.toObject(), IPS_STR("name"), IPS_STR("New Kew1")) == IPS_STR("New Kew1"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("function"), IPS_STR("Duplex")) == IPS_STR("Duplex"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("destination"), IPS_STR("CID101")) == IPS_STR("CID101"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("priority"), IPS_STR("60")) == IPS_STR("60"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("controlLine"), IPS_STR("ActvateControlLine")) == IPS_STR("ActvateControlLine"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("pre_tone"), IPS_STR("0")) == IPS_STR("0"));
            REQUIRE(json::getString(f.toObject(), IPS_STR("pre_tone_duration"), IPS_STR("")) == IPS_STR(""));
    }
    SECTION("Check deserialize") {
            REQUIRE(testItem.deserializeModel(jsonItems));
    }
}
