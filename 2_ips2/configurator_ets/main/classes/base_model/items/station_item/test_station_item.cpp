#include <QString>
#include <QTest>
//#include <QtWidgets>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_station_item.h"

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

Test_StationItem::Test_StationItem(QWidget *parent)
    : QObject(parent)
{
	pStationItem = new StationItem();
}

Test_StationItem::~Test_StationItem()
{
	delete pStationItem;
}

void Test_StationItem::Test_AddKey()
{
	while (pStationItem->getModel()->rowCount()) {
		pStationItem->DeleteKeys(0, 0);
	}
	pStationItem->getModel()->setStationType(StationType::PDO16);
	pStationItem->getModel()->insertRows(0, 1);
	QCOMPARE(pStationItem->getModel()->rowCount(), 1);
}

void Test_StationItem::Test_RemoveKey()
{
	int _rowCount = 0;
	if (pStationItem->getModel()->rowCount() == 0)
		pStationItem->getModel()->insertRows(0, 1);
	_rowCount = pStationItem->getModel()->rowCount();
	pStationItem->getModel()->removeRow(0);
	QCOMPARE(pStationItem->getModel()->rowCount(), _rowCount-1);
}

void Test_StationItem::Test_SerializeStation()
{
	while (pStationItem->getModel()->rowCount()) {
		pStationItem->DeleteKeys(0, 0);
	}
	pStationItem->getModel()->setStationType(StationType::PDO16);
	pStationItem->getModel()->insertRows(0, 1);
	QJsonArray arr = pStationItem->getModel()->serialize();
	QCOMPARE(arr.count(), 1);
}

void Test_StationItem::Test_DeserializeStation()
{
	while (pStationItem->getModel()->rowCount()) {
		pStationItem->getModel()->removeRow(0);
	}
	pStationItem->getModel()->setStationType(StationType::PDO16);
	pStationItem->getModel()->insertRows(0, 1);
	QJsonArray arr = pStationItem->getModel()->serialize();
	while (pStationItem->getModel()->rowCount()) {
		pStationItem->getModel()->removeRow(0);
	}
	pStationItem->getModel()->deserialize(arr);
	QCOMPARE(pStationItem->getModel()->rowCount(), 1);
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
