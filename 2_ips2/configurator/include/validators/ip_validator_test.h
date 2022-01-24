#pragma once
#include <catch.hpp>
#include <QValidator>
#include <QWidget>
#include <QRegExp>
#include <QRegExpValidator>
#include "../include/validators/ip_validator.h"
#include "classes/base_model/items/test_classes.h"
#include "classes/base_model/items/stations/station_items.h"

TEST_CASE("Test case for IpValidator") {
    IpValidator valid;
    auto const& range = IPS_STR("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])");
    auto const& rangeError = IPS_STR("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9])");
    auto reg = QRegExp(IPS_STR("^%1\\.%1\\.%1\\.%1$").arg(range));
    auto regError = QRegExp(IPS_STR("^%1\\.%1\\.%1\\.%1$").arg(rangeError));
    reg.setCaseSensitivity(Qt::CaseInsensitive);
    reg.setPatternSyntax(QRegExp::RegExp);
    regError.setCaseSensitivity(Qt::CaseInsensitive);
    regError.setPatternSyntax(QRegExp::RegExp);

    auto strError = IPS_STR("12345");
    auto str = IPS_STR("192.168.10.1");

    SECTION("Check validator for function ip") {
            REQUIRE(valid.ip() == reg);
            REQUIRE(valid.ip() != regError);

    }
    SECTION("Check validator for function check") {
            REQUIRE(valid.check(strError) == QValidator::State::Invalid);
            REQUIRE(valid.check(str) == QValidator::State::Acceptable);

    }
}
