#pragma once
#include <catch.hpp>
#include <QValidator>
#include <QWidget>
#include "classes/base_model/items/test_classes.h"
#include "../include/validators/string_validator.h"
#include "classes/base_model/items/stations/station_items.h"

TEST_CASE("Test case for StringSizeValidator") {
    TestValidator testValid(nullptr);
    auto string = IPS_STR("123456789");
    auto strLong = IPS_STR("123456789101112131415");
    auto strShort = IPS_STR("12345");
    auto emptyString = IPS_STR("");
    auto pos = -1;

    SECTION("Check validator for function toValid") {
            REQUIRE(testValid.valid()->toValid(string) == IPS_STR("12345"));
            REQUIRE(testValid.valid()->toValid(string) != IPS_STR("123456789"));
            REQUIRE(testValid.valid()->toValid(emptyString) == IPS_STR(""));
            REQUIRE(testValid.valid()->toValid(emptyString) != IPS_STR("123456789"));
    }
    SECTION("Check validator for function validate") {
            REQUIRE(testValid.valid()->validate(string, pos) == QValidator::State::Invalid);
            REQUIRE(testValid.valid()->validate(strLong, pos) == QValidator::State::Invalid);
            REQUIRE(testValid.valid()->validate(strShort, pos) == QValidator::State::Acceptable);
            REQUIRE(testValid.valid()->validate(emptyString, pos) == QValidator::State::Acceptable);
    }
}
