/*!
 * @file test_debugviewdlg.h
 * @file test_debugviewdlg.cpp
 * @brief This file contains a test class for testing debug viewer module
*/
#ifndef TEST_DEBUGVIEWDLG_H
#define TEST_DEBUGVIEWDLG_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "debugviewdlg.h"
/*! 
\defgroup test_debugview_module Test class for the test message viewer
*/
class Test_DebugViewDialog final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_DebugViewDialog;
 public:
/*!
 \fn Test_DebugViewDialog(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_DebugViewDialog(QWidget* parent = nullptr);
/*!
 \fn ~Test_DebugViewDialog()
 \brief Function destroy class
*/
    ~Test_DebugViewDialog() override;

private slots:
/*!
 \fn Test_DebugDialog()
 \brief Test function debug viewer module
*/
	void Test_DebugDialog();

private:
///pointer to debug viewer module unit
	DebugViewDialog* pDebugViewDialog;
};
#endif
