/*!
 * @file test_stations_item.h
 * @file test_stations_item.cpp
 * @brief This file contains a test class for testing stations item module
*/
#ifndef TEST_STATIONS_ITEM_H
#define TEST_STATIONS_ITEM_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "model_stations/model_stations.h"
#include "stations_item_settings/stations_item_settings.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "main/messages/model_tracks/tracks_model.h"
#include "stations_item.h"
/*! 
\defgroup test_stations_item_module Test class for the test stations item
*/
class Test_StationsItem final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_StationsItem;
 public:
/*!
 \fn Test_StationsItem(QWidget* parent = nullptr)
 \brief Function creation class
 \param[in] parent pointer to the parent class
*/
    explicit Test_StationsItem(QWidget* parent = nullptr);
/*!
 \fn ~Test_StationsItem()
 \brief Function destroy class
*/
    ~Test_StationsItem() override;

private slots:
/*!
 \fn Test_AddStations()
 \brief Stations add test function
*/
	void Test_AddStations();
/*!
 \fn Test_RemoveStations()
 \brief Stations removal test function  
*/
	void Test_RemoveStations();
/*!
 \fn Test_SerializeStations()
 \brief Stations parameter serialization function    
*/
	void Test_SerializeStations();
/*!
 \fn Test_DeserializeStations()
 \brief Stations parameter deserialization function    
*/
	void Test_DeserializeStations();

private:
///pointer to debug viewer module unit
	StationsItem* pStationsItem;
};
#endif
