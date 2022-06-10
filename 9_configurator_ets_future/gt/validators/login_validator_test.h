/**
 * \file      ojects/cnf/include/validators/login_validator_test.h 
 * \brief     The Login_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 06(th), 2020, 13:15 MSK
 * \updated   November  (the) 06(th), 2020, 13:15 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "login_validator.h"

TEST_CASE("Test case for LoginValidator") {
    AssertTrue(gt::LoginValidator::isValid(RT_STR("hello")));
    AssertFalse(gt::LoginValidator::isValid(RT_STR("привет")));
    AssertFalse(gt::LoginValidator::isValid(RT_STR("helloя")));
    AssertFalse(gt::LoginValidator::isValid(RT_STR("2he43920o")));
}
TEST_CASE("Test case for LoginValidator Invalid") {
    gt::LoginValidator v;
    QString str;
    int pos = -1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    pos = 1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
TEST_CASE("Test case for LoginValidator Valid") {
    gt::LoginValidator v;
    QString str = RT_STR("hello");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    pos = 4;
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);
}
TEST_CASE("Test case for LoginValidator position") {
    gt::LoginValidator v;
    QString str = RT_STR("hello-я");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    str = RT_STR("1hello-я");
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
