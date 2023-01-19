#include "appcore.h"
#include <QTimer>
#include <QFile>
#include <qdatastream.h>
#include <QSqlRecord>
#include <QModelIndex>
#include <QThread>
#include <QDir>

void AppCore::getListOfStationsAndGroups()
{
    for (const auto &listOfStation : listOfStations) {
        QString itemForAddingTrim = listOfStation.nameOfID;
        itemForAddingTrim.remove(0,4);
        emit listOfStationsAndGroups(listOfStation.nameOfID, listOfStation.nameOfStation, itemForAddingTrim);
    }

    for (const auto &listOfGroup : listOfGroups) {
        QString itemForAddingTrim = listOfGroup.nameOfID;
        emit listOfStationsAndGroups(listOfGroup.nameOfID, listOfGroup.nameOfGorup, itemForAddingTrim);
    }
}

void AppCore::getListOfStationsRs232()
{
    for (const auto &listOfStation : listOfStations) {
        QString itemForAddingTrim = listOfStation.nameOfID;
        itemForAddingTrim.remove(0,4);
        emit listOfStationsAndGroups(listOfStation.nameOfID, listOfStation.nameOfStation, itemForAddingTrim);
    }
}

void AppCore::getCurrentNameOfStationsAndGroups(const QString &nameItem)
{
    emit sendToQmlSubscriberAssignment(nameItem);
}

void AppCore::getCurrentNameOfStationsRs232(const QString &nameItem)
{
    emit sendToQmlSubscriberAssignmentRs232(nameItem);
}
