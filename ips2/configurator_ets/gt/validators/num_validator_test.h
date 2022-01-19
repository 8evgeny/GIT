/**
 * \file      ojects/cnf/include/validators/num_validator_test.h 
 * \brief     The Num_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 14:01 MSK
 * \updated   November  (the) 19(th), 2020, 14:01 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "num_validator.h"

TEST_CASE("Test case for NumValidator Invalid") {
    gt::NumValidator v;
    QString str;
    int pos = -1;
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    pos = 1;
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);
}
TEST_CASE("Test case for NumValidator Valid") {
    gt::NumValidator v;
    QString str = RT_STR("1");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    str = RT_STR("100");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    str = RT_STR("10000");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);
}
TEST_CASE("Test case for NumValidator Invalid") {
    gt::NumValidator v;
    QString str = RT_STR("");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("1");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    str = RT_STR("-100");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
