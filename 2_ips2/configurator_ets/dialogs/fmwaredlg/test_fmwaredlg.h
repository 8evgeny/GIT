/*!
 * @file test_fmwaredlg.h
 * @file test_fmwaredlg.cpp
 * @brief This file contains a test class for testing firmware module
*/
#ifndef TEST_FMWAREDLG_H
#define TEST_FMWAREDLG_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "fmwaredlg.h"
/*! 
\defgroup test_configuration_module Test class for the firmware module
*/
class Test_FmwareDialog final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_FmwareDialog;
 public:
 /*!
 \fn Test_FmwareDialog(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_FmwareDialog(QWidget* parent = nullptr);
/*!
 \fn ~Test_FmwareDialog()
 \brief Function destroy class
*/
    ~Test_FmwareDialog() override;

private slots:
/*!
 \fn Test_Constructions()
 \brief Test function constructions interface
*/
	void Test_Constructions();
/*!
 \fn Test_SendFmware()
 \brief Test the function of transferring software image to the device
*/
	void Test_SendFmware();

private:
///pointer to firmware module unit
	FmWareDialog* pFmWareDlg;
};
#endif
