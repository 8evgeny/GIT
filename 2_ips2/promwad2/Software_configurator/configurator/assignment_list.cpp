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
        emit listOfStationsAndGroups(listOfStation.nameOfID, listOfStation.nameOfStation);
    }

    for (const auto &listOfGroup : listOfGroups) {
        emit listOfStationsAndGroups(listOfGroup.nameOfID, listOfGroup.nameOfGorup);
    }
}

void AppCore::getListOfStationsRs232()
{
    for (const auto &listOfStation : listOfStations) {
        emit listOfStationsAndGroups(listOfStation.nameOfID, listOfStation.nameOfStation);
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
