#pragma once
#include <run_tests/run_tests.h>
#include "double.h"
#include "json.h"

class FakeJson {
 public:
    static QJsonObject fakeObject() {
        return {
            {RT_STR("login"), RT_STR("login-1")}
          , {RT_STR("int"), 55}
          , {RT_STR("admin"), static_cast<int>(11U)}
          , {RT_STR("bool"), true}
          , {RT_STR("double"), 1.2}
          , {RT_STR("array"), QJsonArray{
              RT_STR("1"), RT_STR("2"), RT_STR("3")
            }
          }
          , {RT_STR("object"), QJsonObject{{
              RT_STR("1"), RT_STR("2")
            }}
          }
        };
    }
};

TEST_CASE("Test Json::toString function") {
    bool is = true;
    AssertTrue(gt::json::toString(FakeJson::fakeObject(), RT_STR("empty"), &is).isEmpty());
    AssertFalse(is);

    AssertEquals(gt::json::toString(FakeJson::fakeObject(), RT_STR("bool"), RT_STR("def"), &is), RT_STR("def"));
    AssertFalse(is);

    AssertEquals(gt::json::toString(FakeJson::fakeObject(), RT_STR("login"), &is), RT_STR("login-1"));
    AssertTrue(is);
}
TEST_CASE("Test Json::toInt function") {
    bool is = true;
    gt::json::toInt(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    AssertEquals(gt::json::toInt(FakeJson::fakeObject(), RT_STR("bool"), -5, &is), -5);
    AssertFalse(is);

    AssertEquals(gt::json::toInt(FakeJson::fakeObject(), RT_STR("int"), &is), 55);
    AssertTrue(is);
}
TEST_CASE("Test Json::toUInt function") {
    bool is = true;
    gt::json::toUint(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    AssertEquals(gt::json::toUint(FakeJson::fakeObject(), RT_STR("bool"), 5, &is), 5);
    AssertFalse(is);

    AssertEquals(gt::json::toUint(FakeJson::fakeObject(), RT_STR("int"), &is), 55);
    AssertTrue(is);
}
TEST_CASE("Test Json::toDouble function") {
    bool is = true;
    gt::json::toDouble(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    AssertTrue(gt::isEqual(gt::json::toDouble(FakeJson::fakeObject(), RT_STR("bool"), 5.3, &is), 5.3));
    AssertFalse(is);

    AssertTrue(gt::isEqual(gt::json::toDouble(FakeJson::fakeObject(), RT_STR("int"), 55, &is), 55));
    AssertTrue(is);

    AssertTrue(gt::isEqual(gt::json::toDouble(FakeJson::fakeObject(), RT_STR("int"), &is), 55));
    AssertTrue(is);
}
TEST_CASE("Test Json::toBool function") {
    bool is = true;
    gt::json::toBool(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    AssertFalse(gt::json::toBool(FakeJson::fakeObject(), RT_STR("int"), false, &is));
    AssertFalse(is);

    AssertTrue(gt::json::toBool(FakeJson::fakeObject(), RT_STR("bool"), &is));
    AssertTrue(is);
}
TEST_CASE("Test Json::toObject function") {
    bool is = true;
    gt::json::toObject(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    auto o = QJsonObject{{RT_STR("555"), RT_STR("563")}};
    AssertEquals(gt::json::toObject(FakeJson::fakeObject(), RT_STR("bool"), o, &is), o);
    AssertFalse(is);

    o = QJsonObject{{ RT_STR("1"), RT_STR("2")}};
    AssertEquals(gt::json::toObject(FakeJson::fakeObject(), RT_STR("object"), &is), o);
    AssertTrue(is);
}
TEST_CASE("Test Json::toArray function") {
    bool is = true;
    gt::json::toArray(FakeJson::fakeObject(), RT_STR("empty"), &is);
    AssertFalse(is);

    auto arr = QJsonArray{RT_STR("555"), RT_STR("563")};
    AssertEquals(gt::json::toArray(FakeJson::fakeObject(), RT_STR("bool"), arr, &is), arr);
    AssertFalse(is);

    arr = QJsonArray{ RT_STR("1"), RT_STR("2"), RT_STR("3")};
    AssertEquals(gt::json::toArray(FakeJson::fakeObject(), RT_STR("array"), &is), arr);
    AssertTrue(is);
}
