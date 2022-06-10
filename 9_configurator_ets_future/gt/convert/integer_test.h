/**
 * \file      ibgt/sources/gt/convert/double_test.h 
 * \brief     The Double_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   December  (the) 02(th), 2020, 11:53 MSK
 * \updated   December  (the) 02(th), 2020, 11:53 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "integer.h"

TEST_CASE("Test gt::convert toDouble function") {
    AssertEquals(gt::convert::toDouble(RT_STR("55")), 55.0);
    AssertEquals(gt::convert::toDouble(RT_STR("xxx55")), 0.0);
    AssertEquals(gt::convert::toDouble(RT_STR("xxx55"), 33), 33.0);

    AssertEquals(gt::convert::toDouble(QVariant(RT_STR("x55"))), 0);
    AssertEquals(gt::convert::toDouble(QVariant(RT_STR("x55")), 22), 22);
    AssertEquals(gt::convert::toDouble(QVariant(RT_STR("55"))), 55);
    AssertEquals(gt::convert::toDouble(QVariant(55)), 55);
    AssertEquals(gt::convert::toDouble(QVariant(55.0)), 55);
}
TEST_CASE("Test gt::convert toInt function") {
    AssertEquals(gt::convert::toInt(RT_STR("55")), 55.0);
    AssertEquals(gt::convert::toInt(RT_STR("xxx55")), 0.0);
    AssertEquals(gt::convert::toInt(RT_STR("xxx55"), 33), 33.0);

    AssertEquals(gt::convert::toInt(QVariant(RT_STR("x55"))), 0);
    AssertEquals(gt::convert::toInt(QVariant(RT_STR("x55")), 22), 22);
    AssertEquals(gt::convert::toInt(QVariant(RT_STR("55"))), 55);
    AssertEquals(gt::convert::toInt(QVariant(55)), 55);
    AssertEquals(gt::convert::toInt(QVariant(55.0)), 55);
}
