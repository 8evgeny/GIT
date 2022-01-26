/**
 * \file      c:/projects/niokr/conan/conan-libgt/email_validator_test.h 
 * \brief     The Email_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 21:38 MSK
 * \updated   November  (the) 19(th), 2020, 21:38 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "email_validator.h"

TEST_CASE("Test case for EamilValidator") {
    AssertTrue(gt::EmailValidator::isValid(RT_STR("dix75@mail.com.ru")));
    AssertFalse(gt::EmailValidator::isValid(RT_STR("dix75mail.com.ru")));
    AssertFalse(gt::EmailValidator::isValid(RT_STR("dix@75m@ail.com.ru")));
}

