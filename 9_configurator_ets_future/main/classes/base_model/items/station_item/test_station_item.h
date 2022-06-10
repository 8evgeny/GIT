/*!
 * @file test_station_item.h
 * @file test_station_item.cpp
 * @brief This file contains a test class for testing station item module
*/
#ifndef TEST_STATION_ITEM_H
#define TEST_STATION_ITEM_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "model_keys/model_keys.h"
#include "station_item_settings/station_item_settings.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "main/messages/model_tracks/tracks_model.h"
#include "main/classes/base_types/station_types.h"
#include "station_item.h"
/*! 
\defgroup test_station_item_module Test class for the test station item
*/
class Test_StationItem final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_StationItem;
 public:
/*!
 \fn Test_StationItem(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_StationItem(QWidget* parent = nullptr);
/*!
 \fn ~Test_StationItem()
 \brief Function destroy class
*/
    ~Test_StationItem() override;

private slots:
/*!
 \fn Test_AddKey()
 \brief Button add test function 
*/
	void Test_AddKey();
/*!
 \fn Test_RemoveKey()
 \brief Button removal test function  
*/
	void Test_RemoveKey();
/*!
 \fn Test_SerializeStation()
 \brief Station parameter serialization function  
*/
	void Test_SerializeStation();
/*!
 \fn Test_DeserializeStation()
 \brief Station parameter deserialization function  
*/
	void Test_DeserializeStation();

private:
///pointer to station item unit
	StationItem* pStationItem;
};
#endif
