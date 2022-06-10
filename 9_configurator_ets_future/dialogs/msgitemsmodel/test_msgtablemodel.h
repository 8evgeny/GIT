/*!
\file test_msgtablemodel.h
\brief This file contains a test class for testing msgtablemodel module
*/
/*! 
@defgroup test_msgtablemodel_module testing class of the message list data display model 
*/
#ifndef TEST_MSGTABLEMODEL_H
#define TEST_MSGTABLEMODEL_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "msgtablemodel.h"
/** @addtogroup test_msgtablemodel_module */
/*@{*/
class Test_MsgTableModel final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_MsgTableModel;
 public:
/*!
 \fn Test_MsgTableModel(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_MsgTableModel(QWidget* parent = nullptr);
/*!
 \fn ~Test_MsgTableModel()
 \brief Function destroy class
*/
    ~Test_MsgTableModel() override;

private slots:
/*!
 \fn Test_TableModel()
 \brief Function testing class MsgTableModel
*/
	void Test_TableModel();

private:
///Pointer to MsgTableModel module
	MsgTableModel* pMsgTableModel;
};
/*@}*/
#endif
