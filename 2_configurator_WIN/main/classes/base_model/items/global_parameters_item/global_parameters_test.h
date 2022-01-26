#pragma once
#include <catch.hpp>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/global_parameters_item/global_parameters.h"

TEST_CASE("Test case for serialize/deserialize") {
        BaseModel model;
        auto item = model.globalParametersItem();

        item->setCallerId(1000);
        item->setDirectconnection(QStringLiteral("defaultDirectconnection"));
        item->setPhonecall(QStringLiteral("defaultPhonecall"));
        item->setGroupcall(QStringLiteral("defaultGroupcall"));
        item->setRangemax(QStringLiteral("defaultRangemax"));
        item->setRangemin(QStringLiteral("defaultRangemin"));
        item->setMessage(QStringLiteral("defaultMessage"));
        item->setConference(QStringLiteral("defaultConference"));
        item->setCircularcall(QStringLiteral("defaultCircularcall"));

        QJsonObject jsonItem = {
                {QStringLiteral("directconnection"), item->directConnection()}
                , {QStringLiteral("groupcall"), item->groupcall()}
                , {QStringLiteral("phonecall"), item->phonecall()}
                , {QStringLiteral("circularcall"), item->circularcall()}
                , {QStringLiteral("conference"), item->conference()}
                , {QStringLiteral("message"), item->message()}
                , {QStringLiteral("callerid"), QString::number(item->callerid())}
                , {QStringLiteral("rangemin"), item->rangemin()}
                , {QStringLiteral("rangemax"), item->rangemax()}
        };
        QJsonObject jsonFull = {{QStringLiteral("Global paramenters"), jsonItem}};

        QJsonObject jsonItemNoValid = {
                {QStringLiteral("direct"), item->directConnection()}
                , {QStringLiteral("groupc"), item->groupcall()}
                , {QStringLiteral("phonec"), item->phonecall()}
                , {QStringLiteral("circularc"), item->circularcall()}
                , {QStringLiteral("confer"), item->conference()}
                , {QStringLiteral("mess"), item->message()}
                , {QStringLiteral("caller"), QString::number(item->callerid())}
                , {QStringLiteral("min"), item->rangemin()}
                , {QStringLiteral("max"), item->rangemax()}
        };
        QJsonObject jsonFullNoValid = {{QStringLiteral("Global paramenters"), jsonItemNoValid}};

        SECTION("serialize()") {
               REQUIRE(item->serialize() == jsonItem);
        }
        SECTION("deserialize()") {
               REQUIRE(item->deserialize(jsonFull));
        }
        SECTION("deserialize()") {
               REQUIRE_FALSE(item->deserialize(jsonFullNoValid));
        }

        auto emptyItem = model.globalParametersItem();

        emptyItem->setCallerId(1000);
        emptyItem->setDirectconnection(QStringLiteral(""));
        emptyItem->setPhonecall(QStringLiteral(""));
        emptyItem->setGroupcall(QStringLiteral(""));
        emptyItem->setRangemax(QStringLiteral(""));
        emptyItem->setRangemin(QStringLiteral(""));
        emptyItem->setMessage(QStringLiteral(""));
        emptyItem->setConference(QStringLiteral(""));
        emptyItem->setCircularcall(QStringLiteral(""));

        QJsonObject jsonEmptyItem = {
                {QStringLiteral("directconnection"), emptyItem->directConnection()}
                , {QStringLiteral("groupcall"), emptyItem->groupcall()}
                , {QStringLiteral("phonecall"), emptyItem->phonecall()}
                , {QStringLiteral("circularcall"), emptyItem->circularcall()}
                , {QStringLiteral("conference"), emptyItem->conference()}
                , {QStringLiteral("message"), emptyItem->message()}
                , {QStringLiteral("callerid"), QString::number(emptyItem->callerid())}
                , {QStringLiteral("rangemin"), emptyItem->rangemin()}
                , {QStringLiteral("rangemax"), emptyItem->rangemax()}
        };
        QJsonObject jsonEmptyFull = {{QStringLiteral("Global paramenters"), jsonEmptyItem}};

        QJsonObject jsonEmptyItemNoValid = {
                {QStringLiteral("direct"), emptyItem->directConnection()}
                , {QStringLiteral("groupc"), emptyItem->groupcall()}
                , {QStringLiteral("phonec"), emptyItem->phonecall()}
                , {QStringLiteral("circularc"), emptyItem->circularcall()}
                , {QStringLiteral("conference"), emptyItem->conference()}
                , {QStringLiteral("message"), emptyItem->message()}
                , {QStringLiteral("callerid"), QString::number(emptyItem->callerid())}
                , {QStringLiteral("rangemin"), emptyItem->rangemin()}
                , {QStringLiteral("rangemax"), emptyItem->rangemax()}
        };
        QJsonObject jsonEmptyNoValid = {{QStringLiteral("Global paramenters"), jsonEmptyItemNoValid}};

        SECTION("serialize()") {
                REQUIRE(emptyItem->serialize() == jsonEmptyItem);
        }
        SECTION("deserialize()") {
                REQUIRE(emptyItem->deserialize(jsonEmptyFull));
        }
        SECTION("deserialize()") {
                REQUIRE_FALSE(item->deserialize(jsonEmptyNoValid));
        }
}
