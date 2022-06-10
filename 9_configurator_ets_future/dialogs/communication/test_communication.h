/*!
\file
\brief test_communication.h

This file contains a test class for testing communication module

*/
/*! 
\defgroup communication_module communication class with the device 
*/
#ifndef TEST_COMMUNICATION_H
#define TEST_COMMUNICATION_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "communication.h"

class Test_Communication final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_Communication;
 public:
/*!
 \fn Test_Communication(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_Communication(QWidget* parent = nullptr);
/*!
 \fn ~Test_Communication()
 \brief Function destroy class
*/
    ~Test_Communication() override;

signals:
/*!
 \fn void sigCancel()
 \brief Function emited signal for canceled current transaction
*/
	void sigCancel();

private slots:
/*!
 \fn void CheckCRC32()
 \brief Test function checksum array data
*/
	void CheckCRC32();
/*!
 \fn void add2Log()
 \brief Test function add2Log
*/
	void add2Log();
/*!
 \fn void Data2Hex()
 \brief Test function Data2Hex
*/
	void Data2Hex();
/*!
 \fn void fnGetCFGUI()
 \brief Test function fnGetCFGUI
*/
	void fnGetCFGUI();
/*!
 \fn void fnConfigOn()
 \brief Test function fnConfigOn
*/
	void fnConfigOn();
/*!
 \fn void fnConfigOff()
 \brief Test function fnConfigOff
*/
	void fnConfigOff();
/*!
 \fn void fnConfigSetBlock()
 \brief Test function fnConfigSetBlock
*/
	void fnConfigSetBlock();
/*!
 \fn void fnConfigGetBlock()
 \brief Test function fnConfigGetBlock
*/
	void fnConfigGetBlock();
/*!
 \fn void fnConfigStartWrite()
 \brief Test function fnConfigStartWrite
*/
	void fnConfigStartWrite();
/*!
 \fn void fnFwwareOn()
 \brief Test function fnFwwareOn
*/
	void fnFwwareOn();
/*!
 \fn void fnFwwareOff()
 \brief Test function fnFwwareOff
*/
	void fnFwwareOff();
/*!
 \fn void fnFwwareSetBlock()
 \brief Test function fnFwwareSetBlock
*/
	void fnFwwareSetBlock();
/*!
 \fn void fnFwwareGetBlock()
 \brief Test function fnFwwareGetBlock
*/
	void fnFwwareGetBlock();
/*!
 \fn void fnFwwareStartWrite()
 \brief Test function fnFwwareStartWrite
*/
	void fnFwwareStartWrite();

private:
///Pointer to communication module
	CommunicationDialog* pCommDlg;
};
#endif
