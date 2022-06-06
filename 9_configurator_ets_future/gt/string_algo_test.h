/**
 * \file      ojects/cnf/include/string_algo_test.h 
 * \brief     The String_algo_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 17:29 MSK
 * \updated   November  (the) 19(th), 2020, 17:29 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "string_algo.h"

TEST_CASE("Test gt::trim function") {
    QLineEdit e;
    e.setText(RT_STR(""));
    AssertEquals(gt::trim(&e), QString{});

    e.setText(RT_STR("    Cool    "));
    AssertEquals(gt::trim(&e), RT_STR("Cool"));
}
