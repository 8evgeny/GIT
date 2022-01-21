/**
 * \file      ibgt/sources/gt/convert/variant_test.h
 * \brief     The Double_test class provides
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020
 * \version   v.1.0
 * \created   December  (the) 02(th), 2020, 11:53 MSK
 * \updated   December  (the) 02(th), 2020, 11:53 MSK
 * \TODO
**/
#pragma once
#include <QVariant>
#include <run_tests/run_tests.h>
#include "string.h"

TEST_CASE("Test gt::convert::toInt function") {
    AssertEquals(gt::convert::toString(QVariant(RT_STR("x55"))), RT_STR("x55"));
    AssertEquals(gt::convert::toString(QVariant(55), RT_STR("663")), RT_STR("55"));
    AssertEquals(gt::convert::toString(QVariant(true)), RT_STR("true"));
    AssertEquals(gt::convert::toString(QVariant(55.5)), RT_STR("55.5"));
}
