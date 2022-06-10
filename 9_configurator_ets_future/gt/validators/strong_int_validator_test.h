/**
 * \file      onan/conan-libgt/sources/gt/validators/strong_int_validator_test.h 
 * \brief     The Strong_int_validator_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2021 
 * \version   v.1.0
 * \created   March     (the) 04(th), 2021, 09:25 MSK
 * \updated   March     (the) 04(th), 2021, 09:25 MSK
 * \TODO      
**/
#pragma once
#pragma once
#include <run_tests/run_tests.h>
#include "strong_int_validator.h"

TEST_CASE("Test case for EamilValidator") {
    AssertFalse(gt::StrongIntValidator::isValid(RT_STR("dix75@mail.com.ru"), 1, 10));
    AssertTrue(gt::StrongIntValidator::isValid(RT_STR("55"), 1, 56));
    AssertFalse(gt::StrongIntValidator::isValid(RT_STR("55"), 1, 55));
    AssertFalse(gt::StrongIntValidator::isValid(RT_STR("x"), 1, 22));
}
