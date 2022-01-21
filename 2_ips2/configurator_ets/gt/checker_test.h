/**
 * \file      ojects/cnf/include/checker_test.h 
 * \brief     The Checker_test class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 16:36 MSK
 * \updated   November  (the) 19(th), 2020, 16:36 MSK
 * \TODO      
**/
#pragma once
#include <run_tests/run_tests.h>
#include "checker.h"

TEST_CASE("Test gt::checker::textSize function") {
    QLineEdit e;
    e.setText(RT_STR(""));
    AssertTrue(gt::checker::textSize(&e, 0, 5));

    e.setText(QString(7, QLatin1Char('1')));
    AssertFalse(gt::checker::textSize(&e, 0, 5));

    e.setText(QString(5, QLatin1Char('1')));
    AssertTrue(gt::checker::textSize(&e, 0, 5));
}
TEST_CASE("Test gt::checker::login function") {
    QLineEdit e;
    e.setText(RT_STR("1"));
    AssertFalse(gt::checker::login(&e));

    e.setText(QString(33, QLatin1Char('a')));
    AssertFalse(gt::checker::login(&e));

    e.setText(QString(32, QLatin1Char('a')));
    AssertTrue(gt::checker::login(&e));
}
TEST_CASE("Test gt::checker::name function") {
    QLineEdit e;
    e.setText(RT_STR("12"));
    AssertFalse(gt::checker::name(&e));

    e.setText(QString(65, QLatin1Char('1')));
    AssertFalse(gt::checker::name(&e));

    e.setText(QString(64, QLatin1Char('1')));
    AssertTrue(gt::checker::name(&e));
}
TEST_CASE("Test gt::checker::Int function") {
    QLineEdit e;
    e.setText(RT_STR("12"));
    AssertFalse(gt::checker::Int(&e, 1, 12));
    AssertTrue(gt::checker::Int(&e, 1, 14));
    AssertTrue(gt::checker::Int(&e, 12, 14));

    e.setText(RT_STR("bo"));
    AssertFalse(gt::checker::Int(&e, 1, 12));
}
TEST_CASE("Test gt::checker::Uint function") {
    QLineEdit e;
    e.setText(RT_STR("12"));
    AssertFalse(gt::checker::Uint(&e, 1, 12));
    AssertTrue(gt::checker::Uint(&e, 1, 14));
    AssertTrue(gt::checker::Uint(&e, 12, 14));

    e.setText(RT_STR("bo"));
    AssertFalse(gt::checker::Uint(&e, 1, 12));
}
TEST_CASE("Test gt::checker::Double function") {
    QLineEdit e;
    e.setText(RT_STR("12"));
    AssertFalse(gt::checker::Double(&e, 1, 12));
    AssertTrue(gt::checker::Double(&e, 1, 14));
    AssertTrue(gt::checker::Double(&e, 12, 14));

    e.setText(RT_STR("bo"));
    AssertFalse(gt::checker::Double(&e, 1, 12));
}
