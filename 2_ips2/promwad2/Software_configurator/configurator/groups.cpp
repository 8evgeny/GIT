#include "appcore.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Lib/qjson/include/QJson/Parser"
#include "Lib/qjson/include/QJson/Serializer"
#include "Lib/qjson/src/cmdlineparser.h"
#include "Lib/qjson/src/qjson_debug.h"

#include <QFile>

void AppCore::addNewStationToGroup(const QString &nameOfGroup, const QString &nameOfStation)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfGroup = 0;

    bool isStation = false;
    bool isGroup = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == nameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //look for an index of the group
    for (int i = 0; i < listOfGroups.size(); i++) {
        if (listOfGroups.at(i).nameOfGorup == nameOfGroup) {
            indexOfGroup = i;
            isGroup = true;
            break;
        }
    }

    if (!isGroup) {
        //there is no such group in the list
        return;
    }

    for (const auto &station : listOfGroups.at(indexOfGroup).stations) {
        if (station.nameOfID == nameOfStation) {
            // error, such station already exists
            return;
        }
    }

    //add new member to the list
    listOfGroups[indexOfGroup].stations.append(listOfStations.at(indexOfStation));

    updateViewListOfGroups(indexOfGroup);
}

void AppCore::deleteStationFromGroup(const QString &nameOfGroup, const QString &nameOfStation)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfGroup = 0;

    bool isStation = false;
    bool isGroup = false;

    //look for an index of the group
    for (int i = 0; i < listOfGroups.size(); i++) {
        if (listOfGroups.at(i).nameOfGorup == nameOfGroup) {
            indexOfGroup = i;
            isGroup = true;
            break;
        }
    }

    if (!isGroup) {
        //there is no such group in the list
        return;
    }


    //look for an index of the station
    for (int i = 0; i < listOfGroups[indexOfGroup].stations.size(); i++) {
        if (listOfGroups[indexOfGroup].stations.at(i).nameOfID == nameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //delete member from the list
    listOfGroups[indexOfGroup].stations.removeAt(indexOfStation);

    updateViewListOfGroups(indexOfGroup);

}

void AppCore::upgradeListOfGroupsIdent(int indexOfGroup, const QString &ident)
{
    //look for an index of the ident
    for (auto &listOfGroup : listOfGroups) {
        if (listOfGroup.nameOfID == ident) {
            emit sendIndexCorrectDataIdentListOfGroups(indexOfGroup, listOfGroups.at(indexOfGroup).nameOfID);
            //error
            return;
        }
    }

    listOfGroups[indexOfGroup].nameOfID = ident;
    emit sendIndexCorrectDataIdentListOfGroups(indexOfGroup, listOfGroups.at(indexOfGroup).nameOfID);
}

void AppCore::upgradeListOfGroupsName(int indexOfGroup, const QString &name)
{
    //look for an index of the ident
    for (auto &listOfGroup : listOfGroups) {
        if (listOfGroup.nameOfGorup == name) {
            //error
            emit sendIndexCorrectDataNameListOfGroups(indexOfGroup, listOfGroups.at(indexOfGroup).nameOfGorup);
            return;
        }
    }

    listOfGroups[indexOfGroup].nameOfGorup = name;
    emit sendIndexCorrectDataNameListOfGroups(indexOfGroup, listOfGroups.at(indexOfGroup).nameOfGorup);
}

void AppCore::updateViewListOfGroups(const QString &nameOfGroup)
{
    int indexOfGroup = 0;
    bool isGroup = false;

    for (int i = 0; i < listOfGroups.size(); i++) {
        if (listOfGroups.at(i).nameOfGorup == nameOfGroup) {
            indexOfGroup = i;
            isGroup = true;
            break;
        }
    }

    if (!isGroup) {
        //there is no such group in the list
        return;
    }

    updateViewListOfGroups(indexOfGroup);
}

void AppCore::updateViewListOfGroups(int indexOfGroup)
{
    //update member list
    emit sendClearListOfStations();
    for (const auto &station : listOfGroups.at(indexOfGroup).stations) {
        emit sendUpdateListOfStations(station.nameOfStation, station.nameOfID);
    }

    //update the list of possible participants
    emit sendClearListOfParts();
    for (const auto &listOfStation : listOfStations) {
        int j = 0;
        for (; j < listOfGroups.at(indexOfGroup).stations.size(); j++) {
            if (listOfStation.nameOfStation
                    == listOfGroups.at(indexOfGroup).stations.at(j).nameOfStation) {
                break;
            }
        }
        if (j == listOfGroups.at(indexOfGroup).stations.size()) {
            emit sendListOfStations(listOfStation.nameOfStation, listOfStation.nameOfID);
        }
    }

    //look for errors (if there is no such station)
    for (int i = 0; i < listOfGroups.at(indexOfGroup).stations.size(); i++) {

        QList<QString> list;
        for (const auto &listOfStation : listOfStations) {
            list.append(listOfStation.nameOfStation);
        }

        QList<QString> listCid;
        for (const auto &listOfStationCid : listOfStations) {
            listCid.append(listOfStationCid.nameOfID);
        }

        // search for element and if found change data
        QString str = listOfGroups.at(indexOfGroup).stations.at(i).nameOfStation;
        int k = list.indexOf(str);

        // search for element and if found change data
        QString strCid = listOfGroups.at(indexOfGroup).stations.at(i).nameOfID;
        int kCid = listCid.indexOf(strCid);

        if ((k != -1) && (kCid != -1)) {
            emit sendNumberOfCorrectIndex(i);
        } else {
            emit sendNumberOfErrorIndex(i); //change color
        }
    }
}

void AppCore::enableVisibleGroups()
{
    emit sendVisibleSubsriber(false);
    emit sendVisibleGroups(true);
}
