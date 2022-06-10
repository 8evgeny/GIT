/*!
 * @file test_serviceviewdlg.h
 * @file test_serviceviewdlg.cpp
 * @brief This file contains a test class for testing service viewer module
*/
#ifndef TEST_SERVICEVIEWDLG_H
#define TEST_SERVICEVIEWDLG_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "serviceviewdlg.h"
/*! 
\defgroup test_serviceview_module Test class for the service message viewer
*/
class Test_ServiceViewDialog final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_ServiceViewDialog;
 public:
/*!
 \fn Test_ServiceViewDialog(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_ServiceViewDialog(QWidget* parent = nullptr);
/*!
 \fn ~Test_ServiceViewDialog()
 \brief Function destroy class
*/
    ~Test_ServiceViewDialog() override;

private slots:
/*!
 \fn Test_ServiceDialog()
 \brief Test function debug viewer module
*/
	void Test_ServiceDialog();

private:
///pointer to service viewer module unit
	ServiceViewDialog* pServiceViewDialog;
};
#endif
