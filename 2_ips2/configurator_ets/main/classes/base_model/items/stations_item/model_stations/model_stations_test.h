#pragma once
#include <catch.hpp>
#include "classes/json.h"
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/test_classes.h"
#include "classes/base_model/items/stations/model_table_stations/model_table_stations.h"

TEST_CASE("Test case ModelTableStations1 class for insett rows") {
    BaseModel model;
    model.insertItem(2, new StationItems(), QModelIndex());
    auto fullItem = model.StationItems();
    TestStationItems itemTest(fullItem);

    REQUIRE(itemTest.insertRows(0, 1));
    REQUIRE_FALSE(itemTest.insertRows(0, 2));
    REQUIRE(itemTest.rowCount() == 9);
}
TEST_CASE("Test case ModelTableStations1 class for serialize/deserialize") {
    BaseModel model;
    model.insertItem(2, new StationItems(), QModelIndex());
    auto fullItem = model.StationItems();
    TestStationItems itemTest(fullItem);

    itemTest.insertRows(0, 1, QModelIndex());

    QJsonObject json = {{IPS_STR("name"), IPS_STR("Name1")}
                       ,{IPS_STR("cid"), IPS_STR("1")}
                       ,{IPS_STR("type"), IPS_STR("PDO32")}
                       ,{IPS_STR("ip"), IPS_STR("192.168.10.1")}
                       ,{IPS_STR("mask"), IPS_STR("255.255.255.0")}};
    QJsonArray jsonArr;
    jsonArr.push_back(json);

    SECTION("Check serialize") {
         auto const& s = itemTest.serializeModel();
         REQUIRE_FALSE(s.isUndefined());
         auto const& f = s.toArray().first();

         REQUIRE(json::getString(f.toObject(), IPS_STR("name"), IPS_STR("Name5")) == IPS_STR("Name5"));
         REQUIRE(json::getString(f.toObject(), IPS_STR("cid"), IPS_STR("5")) == IPS_STR("5"));
         REQUIRE(json::getString(f.toObject(), IPS_STR("type"), IPS_STR("UPW4")) == IPS_STR("UPW4"));
         REQUIRE(json::getString(f.toObject(), IPS_STR("ip"), IPS_STR("192.168.10.1")) == IPS_STR("192.168.10.1"));
         REQUIRE(json::getString(f.toObject(), IPS_STR("mask"), IPS_STR("255.255.255.0")) == IPS_STR("255.255.255.0"));
    }
    SECTION("Check deserialize") {
         REQUIRE(itemTest.deserializeModel(jsonArr));
    }
    SECTION("Check insertRows") {
         REQUIRE(itemTest.insertRows(1, 1));
         REQUIRE_FALSE(itemTest.insertRows(1, 2));
    }
    SECTION("Check Data") {
        itemTest.insertRows(0, 1, QModelIndex());

        QModelIndex index01 = itemTest.indexModel(0, 1);
        QModelIndex index02 = itemTest.indexModel(0, 2);
        QModelIndex index03 = itemTest.indexModel(0, 3);
        QModelIndex index04 = itemTest.indexModel(0, 4);
        QModelIndex index05 = itemTest.indexModel(0, 5);

        REQUIRE(itemTest.data(QModelIndex(), Qt::DisplayRole) == QVariant());
        REQUIRE(itemTest.data(index01, Qt::DisplayRole) == IPS_STR("Name10"));
        REQUIRE(itemTest.data(index02, Qt::DisplayRole) == 10);
        REQUIRE(itemTest.data(index03, Qt::DisplayRole) == IPS_STR("UPW4"));
        REQUIRE(itemTest.data(index04, Qt::DisplayRole) == IPS_STR("192.168.10.1"));
        REQUIRE(itemTest.data(index05, Qt::DisplayRole) == IPS_STR("255.255.255.0"));
    }
    SECTION("Check setData") {
        itemTest.insertRows(0, 1, QModelIndex());

        QModelIndex index01 = itemTest.indexModel(0, 1);
        QModelIndex index02 = itemTest.indexModel(0, 2);
        QModelIndex index03 = itemTest.indexModel(0, 3);
        QModelIndex index04 = itemTest.indexModel(0, 4);
        QModelIndex index05 = itemTest.indexModel(0, 5);

        REQUIRE_FALSE(itemTest.setDataModel(QModelIndex(), QVariant()));
        REQUIRE(itemTest.setDataModel(index01, IPS_STR("Name12")));
        REQUIRE(itemTest.setDataModel(index02, 12));
        REQUIRE(itemTest.setDataModel(index03, IPS_STR("UPW4")));
        REQUIRE(itemTest.setDataModel(index04, IPS_STR("192.168.10.1")));
        REQUIRE(itemTest.setDataModel(index05, IPS_STR("255.255.255.0")));
   }
}

