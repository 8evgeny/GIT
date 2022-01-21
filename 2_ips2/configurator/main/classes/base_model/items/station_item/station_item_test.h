#pragma once
#include <catch.hpp>
#include "classes/types.h"
#include "main_window.h"
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/test_classes.h"
#include "classes/base_model/items/stations/station_item.h"

TEST_CASE("Test case for serialize/deserialize in StationItem") {
    BaseModel model;
    model.insertItem(2, new StationItems(), QModelIndex());
    auto fullItem = model.StationItems();
    LOG_FATAL(fullItem, "fullItem cannot be null");

    TestStationItems itemTest(fullItem);

    itemTest.insertRows(0, 1, QModelIndex());
    itemTest.insertRows(1, 1, QModelIndex());

    QModelIndex index01 = itemTest.indexModel(0, 1);
    QModelIndex index02 = itemTest.indexModel(0, 2);
    QModelIndex index03 = itemTest.indexModel(0, 3);
    QModelIndex index04 = itemTest.indexModel(0, 4);
    QModelIndex index05 = itemTest.indexModel(0, 5);

    QModelIndex index11 = itemTest.indexModel(1, 1);
    QModelIndex index12 = itemTest.indexModel(1, 2);
    QModelIndex index13 = itemTest.indexModel(1, 3);
    QModelIndex index14 = itemTest.indexModel(1, 4);
    QModelIndex index15 = itemTest.indexModel(1, 5);
    itemTest.setData(index01, IPS_STR("TestName"));
    itemTest.setData(index02, itemTest.testStItems()->childCount());
    itemTest.setData(index03, IPS_STR("Name%1").arg(itemTest.testStItems()->childCount()));
    itemTest.setData(index04, IPS_STR("192.168.10.1"));
    itemTest.setData(index05, IPS_STR("255.255.255.0"));

    itemTest.setData(index11, IPS_STR("TestName1"));
    itemTest.setData(index12, 1+itemTest.testStItems()->childCount());
    itemTest.setData(index13, IPS_STR("Name%1").arg(1+itemTest.testStItems()->childCount()));
    itemTest.setData(index14, IPS_STR("192.168.10.2"));
    itemTest.setData(index15, IPS_STR("255.255.255.1"));

    itemTest.insertRowsModelFromChilds(0, 0, 1, QModelIndex());
    itemTest.insertRowsModelFromChilds(1, 1, 1, QModelIndex());

    QModelIndex ind01 = itemTest.indexModelFromChilds(0, 0, 1);
    QModelIndex ind02 = itemTest.indexModelFromChilds(0, 0, 2);
    QModelIndex ind03 = itemTest.indexModelFromChilds(0, 0, 3);
    QModelIndex ind04 = itemTest.indexModelFromChilds(0, 0, 4);

    QModelIndex ind11 = itemTest.indexModelFromChilds(1, 1, 1);
    QModelIndex ind12 = itemTest.indexModelFromChilds(1, 1, 2);
    QModelIndex ind13 = itemTest.indexModelFromChilds(1, 1, 3);
    QModelIndex ind14 = itemTest.indexModelFromChilds(1, 1, 4);

    itemTest.setDataModelFromChilds(0, ind01,  IPS_STR("New Kew1"));
    itemTest.setDataModelFromChilds(0, ind02,  IPS_STR("Duplex"));
    itemTest.setDataModelFromChilds(0, ind03,  IPS_STR("CID101"));
    itemTest.setDataModelFromChilds(0, ind04,  IPS_STR("60"));

    itemTest.setDataModelFromChilds(1, ind11,  IPS_STR("TestName1"));
    itemTest.setDataModelFromChilds(1, ind12,  IPS_STR("Testfunction1"));
    itemTest.setDataModelFromChilds(1, ind13,  IPS_STR("Testdestination1"));
    itemTest.setDataModelFromChilds(1, ind14,  IPS_STR("Testpriority1"));

    auto i = 0;
    QJsonArray jsonItems;
    QJsonArray jsonItemsError;
    for (auto const& var : itemTest.items()) {
            QJsonObject jsonItem = {
                    {IPS_STR("name"), var->nameStation()}
                  , {IPS_STR("cid"), static_cast<int>(var->cid())}
                  , {IPS_STR("name"), var->nameStation()}
                  , {IPS_STR("cid"), static_cast<int>(var->cid())}
                  , {IPS_STR("type"), toString(var->stationType())}
                  , {IPS_STR("ip"), var->ip()}
                  , {IPS_STR("mask"), var->maskstation()}
                  , {IPS_STR("ip"), var->ip()}
                  , {IPS_STR("mask"), var->maskstation()}
                  , {IPS_STR("items"), itemTest.serializeModelChilds(i)}
            };
            QJsonObject jsonItemError = {
                    {IPS_STR("nameError"), var->nameStation()}
                  , {IPS_STR("cidError"), static_cast<int>(var->cid())}
                  , {IPS_STR("nameError"), var->nameStation()}
                  , {IPS_STR("cidError"), static_cast<int>(var->cid())}
                  , {IPS_STR("typeError"), toString(var->stationType())}
                  , {IPS_STR("ipError"), var->ip()}
                  , {IPS_STR("maskError"), var->maskstation()}
                  , {IPS_STR("ipError"), var->ip()}
                  , {IPS_STR("maskError"), var->maskstation()}
                  , {IPS_STR("itemsError"), itemTest.serializeModelChilds(i)}
            };
             jsonItems.push_back(jsonItem);
             jsonItemsError.push_back(jsonItemError);
             ++i;
    }
    QJsonObject jsonFullItem = {{IPS_STR("Stations"), jsonItems}};
    QJsonObject jsonFullItemError = {{IPS_STR("StationsError"), jsonItemsError}};
    SECTION("serialize()") {
        REQUIRE(fullItem->serialize().toArray() == jsonItems);
    }
    SECTION("deserialize()") {
        REQUIRE(fullItem->deserialize(jsonFullItem));
        REQUIRE_FALSE(fullItem->deserialize(jsonFullItemError));
    }
}
