#pragma once
#include <catch.hpp>
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/test_classes.h"
#include "classes/base_model/items/stations/station_item.h"
#include "classes/base_model/items/stations/station_items.h"

TEST_CASE("Test case for serialize/deserialize in StationTypeItem") {
    BaseModel model;
    auto parentItem = model.StationItems();
    LOG_FATAL(parentItem, "parentItem cannot be null");
    parentItem->appendChild(new StationItem(parentItem));
    auto fullItem = dynamic_cast<StationItem*>(model.StationItems()->child(0));
    LOG_FATAL(fullItem, "fullItem cannot be null");

    TestStationItem itemTest(fullItem);

    itemTest.insertRows(0, 1, QModelIndex());
    QModelIndex index01 = itemTest.indexModel(0, 1);
    QModelIndex index02 = itemTest.indexModel(0, 2);
    QModelIndex index03 = itemTest.indexModel(0, 3);
    QModelIndex index04 = itemTest.indexModel(0, 4);

    QJsonArray jsonItems;
    QJsonArray jsonItemsError;

    auto j = 0;
    for (auto const& var : itemTest.itemsActivStations()) {
        QJsonObject jsonItem = {
            {IPS_STR("name"), var.name()}
          , {IPS_STR("function"), var.function()}
          , {IPS_STR("destination"), var.destination()}
          , {IPS_STR("priority"), QString::number(var.priority())}
          , {IPS_STR("pre_tone"), QString::number(itemTest.preton().value(j).mapPretones().m_pre_tone)}
          , {IPS_STR("pre_tone_duration"), itemTest.preton().value(j).mapPretones().m_pre_tone_duration}
          , {IPS_STR("pairs_stations"), var.serialize()}
        };
        QJsonObject jsonItemError = {
            {IPS_STR("nameError"), var.name()}
          , {IPS_STR("functionError"), var.function()}
          , {IPS_STR("destinationError"), var.destination()}
          , {IPS_STR("priorityError"), QString::number(var.priority())}
          , {IPS_STR("pairs_stations"), var.serialize()}
       };
       jsonItems.push_back(jsonItem);
       jsonItemsError.push_back(jsonItemError);
       ++j;
    }
    QJsonArray settings;
    for (auto const& item : itemTest.itemsPairsStations()) {
        settings.push_back(QJsonObject {
           {IPS_STR("line"), QString::number(item.line())}
         , {IPS_STR("station"), item.station()}
        });
    }
    QJsonObject jsonFullItem = {{IPS_STR("activestation"), jsonItems}};
    QJsonObject jsonFullItemError = {{IPS_STR("activestationError"), jsonItemsError}};
    QJsonObject json = {{IPS_STR("items"), jsonFullItem}};
    QJsonObject jsonError = {{IPS_STR("items"), jsonFullItemError}};

    SECTION("serialize()") {
        REQUIRE(fullItem->serialize().toObject() == jsonFullItem);
    }
    SECTION("deserialize()") {
        REQUIRE(fullItem->deserialize(json));
        REQUIRE_FALSE(fullItem->deserialize(jsonError));
    }
    SECTION("Check Data") {
        REQUIRE(itemTest.data(QModelIndex(), Qt::DisplayRole) == QVariant());
        REQUIRE(itemTest.data(index01, Qt::DisplayRole) == IPS_STR("New Kew1"));
        REQUIRE(itemTest.data(index02, Qt::DisplayRole) == IPS_STR("Duplex"));
        REQUIRE(itemTest.data(index03, Qt::DisplayRole) == IPS_STR("CID101"));
        REQUIRE(itemTest.data(index04, Qt::DisplayRole) == 60);
    }
    SECTION("Check setData") {
        REQUIRE_FALSE(itemTest.setDataModel(QModelIndex(), QVariant()));
        REQUIRE(itemTest.setDataModel(index01, IPS_STR("New Kew1")));
        REQUIRE(itemTest.setDataModel(index02, IPS_STR("Duplex")));
        REQUIRE(itemTest.setDataModel(index03, IPS_STR("CID101")));
        REQUIRE(itemTest.setDataModel(index04, 60));
}
}
