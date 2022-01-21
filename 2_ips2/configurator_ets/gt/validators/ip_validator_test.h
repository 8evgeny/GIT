/**
 * \file      c:/projects/niokr/conan/conan-libgt/sources/gt/validators/ip_validator_test.h 
 * \brief     The Ip_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 21:38 MSK
 * \updated   November  (the) 19(th), 2020, 21:38 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "ip_validator.h"

TEST_CASE("Test case for IpValidator") {
    AssertTrue(gt::IpValidator::isValid(RT_STR("1")));
    AssertTrue(gt::IpValidator::isValid(RT_STR("172.17.4.45")));
    AssertFalse(gt::IpValidator::isValid(RT_STR("172.1x7.4.45")));
}
TEST_CASE("Test case for IpValidator Invalid") {
    gt::IpValidator v;
    QString str = RT_STR("");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("1");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("10");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("10.");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("10.22.");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("10.22.22");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Intermediate);

    str = RT_STR("10.22.22.1");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    str = RT_STR("10.22.22.1.");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    str = RT_STR("-100");
    pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
