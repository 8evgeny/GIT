/**
 * \file      ibgt/sources/gt/validators/pass_validator_test.h 
 * \brief     The Pass_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 20:34 MSK
 * \updated   November  (the) 19(th), 2020, 20:34 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "pass_validator.h"

TEST_CASE("Test case for PassValidator") {
    AssertTrue(gt::PassValidator::isValid(RT_STR("hello")));
    AssertFalse(gt::PassValidator::isValid(RT_STR("привет")));
    AssertFalse(gt::PassValidator::isValid(RT_STR("helloя")));
    AssertTrue(gt::PassValidator::isValid(RT_STR("2he43920o")));
    AssertFalse(gt::PassValidator::isValid(RT_STR(" helloя")));
    AssertFalse(gt::PassValidator::isValid(RT_STR("helloя ")));
    AssertFalse(gt::PassValidator::isValid(RT_STR("hel loя")));
}
TEST_CASE("Test case for PassValidator Invalid") {
    gt::PassValidator v;
    QString str;
    int pos = -1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    pos = 1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
TEST_CASE("Test case for PassValidator Valid") {
    gt::PassValidator v;
    QString str = RT_STR("hello");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    pos = 4;
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);
}
TEST_CASE("Test case for PassValidator position") {
    gt::PassValidator v;
    QString str = RT_STR("hello-я");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
