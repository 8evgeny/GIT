/**
 * \file      ojects/cnf/include/validators/english_validator_test.h 
 * \brief     The English_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 06(th), 2020, 12:16 MSK
 * \updated   November  (the) 06(th), 2020, 12:16 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "english_validator.h"

TEST_CASE("Test case for EnglishValidator") {
    AssertTrue(gt::EnglishValidator::isValid(RT_STR("hello")));
    AssertFalse(gt::EnglishValidator::isValid(RT_STR("привет")));
    AssertFalse(gt::EnglishValidator::isValid(RT_STR("helloя")));
    AssertTrue(gt::EnglishValidator::isValid(RT_STR("2he43920o")));
}
TEST_CASE("Test case for EnglishValidator Invalid") {
    gt::EnglishValidator v;
    QString str;
    int pos = -1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    pos = 1;
    AssertEquals(v.validate(str, pos), QValidator::Invalid);
}
TEST_CASE("Test case for EnglishValidator Valid") {
    gt::EnglishValidator v;
    QString str = RT_STR("hello");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);

    pos = 4;
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);
}
TEST_CASE("Test case for EnglishValidator position") {
    gt::EnglishValidator v;
    QString str = RT_STR("hello-я");
    int pos = str.length();
    AssertEquals(v.validate(str, pos), QValidator::Invalid);

    pos = 4;
    AssertEquals(v.validate(str, pos), QValidator::Acceptable);
}
