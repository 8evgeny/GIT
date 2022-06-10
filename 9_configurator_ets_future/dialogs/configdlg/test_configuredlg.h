/*!
 * @file test_configuredlg.h
 * @file test_configuredlg.cpp
 * @brief This file contains a test class for testing configuration module
*/
#ifndef TEST_CONFIGUREDLG_H
#define TEST_CONFIGUREDLG_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "configuredlg.h"
/*! 
\defgroup test_configuration_module Test class for the configuration module
*/
class Test_ConfigureDialog final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_ConfigureDialog;
 public:
 /*!
 \fn Test_ConfigureDialog(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_ConfigureDialog(QWidget* parent = nullptr);
/*!
 \fn ~Test_ConfigureDialog()
 \brief Function destroy class
*/
    ~Test_ConfigureDialog() override;

private slots:
/*!
 \fn Test_Constructions()
 \brief Test function constructions interface
*/
	void Test_Constructions();
/*!
 \fn Test_b_SetSN_clicked()
 \brief Test of the function of setting the serial number of the device 
*/
	void Test_b_SetSN_clicked();
/*!
 \fn Test_b_GetAES_clicked()
 \brief Test of the function of getting the encryption key of the device
*/
	void Test_b_GetAES_clicked();
/*!
 \fn Test_b_SetAES_clicked()
 \brief Test of the function of setting the encryption key of the device 
*/
	void Test_b_SetAES_clicked();
private:
///pointer to configuration module unit
	ConfigureDialog* pConfigDlg;
};
#endif
