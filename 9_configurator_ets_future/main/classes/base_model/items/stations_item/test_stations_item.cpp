#include <QString>
#include <QTest>
//#include <QtWidgets>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_stations_item.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

Test_StationsItem::Test_StationsItem(QWidget *parent)
    : QObject(parent)
{
	pStationsItem = new (StationsItem);
}

Test_StationsItem::~Test_StationsItem()
{
	delete pStationsItem;
}

void Test_StationsItem::Test_AddStations()
{
	while (pStationsItem->model()->rowCount()) {
		pStationsItem->model()->removeRows(0, 1);
	}
	pStationsItem->model()->insertRow(-1);
	QCOMPARE(pStationsItem->model()->rowCount(), 1);
}

void Test_StationsItem::Test_RemoveStations()
{
	int _rowCount = 0;
	if (pStationsItem->model()->rowCount() == 0)
		pStationsItem->model()->insertRow(-1);
	_rowCount = pStationsItem->model()->rowCount();
	pStationsItem->model()->removeRows(0, 1);
	QCOMPARE(pStationsItem->model()->rowCount(), _rowCount-1);
}

void Test_StationsItem::Test_SerializeStations()
{
	while (pStationsItem->model()->rowCount()) {
		pStationsItem->model()->removeRows(0, 1);
	}
	pStationsItem->model()->insertRow(-1);
	QVariantMap map;
	map.insert(IPS_STR("Stations"), pStationsItem->model()->serialize());
	QJsonObject obj = QJsonObject::fromVariantMap(map);
	QJsonArray arr = obj[QString::fromLocal8Bit("Stations")].toArray();
	QCOMPARE(arr.count(), 1);
}

void Test_StationsItem::Test_DeserializeStations()
{
	while (pStationsItem->model()->rowCount()) {
		pStationsItem->model()->removeRows(0, 1);
	}
	pStationsItem->model()->insertRow(-1);
	QVariantMap map;
	map.insert(IPS_STR("Stations"), pStationsItem->model()->serialize());
	QJsonObject obj = QJsonObject::fromVariantMap(map);
	QJsonArray arr = obj[QString::fromLocal8Bit("Stations")].toArray();
	while (pStationsItem->model()->rowCount()) {
		pStationsItem->model()->removeRows(0, 1);
	}
	pStationsItem->model()->deserialize(arr);
	QCOMPARE(pStationsItem->model()->rowCount(), 1);
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
