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

void AppCore::sendCurrentIndexOfDigitalStation(const qint32 &index, const QString &nameOfStation)
{
    //enable visible for subscriber.qml
    emit sendVisibleSubsriber(true);
    emit sendVisibleGroups(false);
    emit sendUpdateListOfStationResize();

    //send current index and name of station for subscribers
    emit sendNameOfStation(index, nameOfStation);

    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

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

    //update information about the station (IP, mask, gateway)
    emit sendInfoNetworkAboutTheStation(
        listOfStations.at(indexOfStation).nameOfStation,
        listOfStations.at(indexOfStation).nameOfID,
        listOfStations.at(indexOfStation).ip,
        listOfStations.at(indexOfStation).maskNetwork,
        listOfStations.at(indexOfStation).gateway);


    //update list of stations for subscriber.qml
    for (const auto &listOfKey : listOfStations.at(indexOfStation).listOfKeys) {
        emit sendListNameOfElements(listOfKey.uidOfKey,
                                    listOfKey.numberOfKey,
                                    listOfKey.nameOfKey,
                                    listOfKey.functionOfKey,
                                    listOfKey.assignedKey);
    }
}

//add a new key block
void AppCore::addNewKeyBlock(const QString &nameOfStation)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

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

    //temporary a list of key
    ListOfkeys tmp;

    //default values for lines
    tmp.uidOfKey = QString::fromStdString(boost::uuids::to_string(boost::uuids::random_generator()()));
    tmp.nameOfKey = "";
    tmp.assignedKey = "";
    tmp.functionOfKey = "";
    tmp.numberOfKey = "";

    //add a new key block to the list
    listOfStations[indexOfStation].listOfKeys.append(tmp);

    //add to the new element to subscriber.qml
    emit sendListNameOfElements(tmp.uidOfKey, tmp.numberOfKey, tmp.nameOfKey, tmp.functionOfKey, tmp.assignedKey);

}

//delete a key block
void AppCore::deleteBlock(const QString &nameOfStation, const QString &uidOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //delete member from the list
    listOfStations[indexOfStation].listOfKeys.removeAt(indexOfKey);
}

//update a number of key
void AppCore::updateKeySubscriberNumberOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &numberOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //check numberOfkey
    qint32 number = numberOfKey.toInt();
    if (!((number >= MIN_KEY_NUMBER) && (number <= MAX_KEY_NUMBER)))
        return;

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].numberOfKey = QString::number(number);

}

//update a name of key
void AppCore::updateKeySubscriberNameOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &nameOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].nameOfKey = nameOfKey;
}

//update a function of key
void AppCore::updateKeySubscriberFunctionOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &functionOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].functionOfKey = functionOfKey;
}

//update a assigned of key
void AppCore::updateKeySubscriberAssignedOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &assignedOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].assignedKey = assignedOfKey;
}


//update a priority of key
void AppCore::updateKeySubscriberPriorityOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &priorityOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].prioriryKey = priorityOfKey;
}

//update a priority of key
void AppCore::updateKeySubscriberModeOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &modeOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    //change a number of key
    listOfStations[indexOfStation].listOfKeys[indexOfKey].modeKey = modeOfKey;
}

//update a priority of key
void AppCore::getSubscriberListPriorityOfKeys(const QString &nameOfStation, const QString &uidOfKey)
{
    //for saving an index
    int indexOfStation = 0;
    int indexOfKey = 0;

    bool isStation = false;
    bool isKey = false;

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

    //look for an index of the key by UID
    for (int i = 0; i < listOfStations.at(indexOfStation).listOfKeys.size(); i++) {
        if (listOfStations.at(indexOfStation).listOfKeys.at(i).uidOfKey == uidOfKey) {
            indexOfKey = i;
            isKey = true;
            break;
        }
    }

    if (!isKey) {
        //key does not exist
        return;
    }

    emit sendInfoPrioriryKeysAboutTheStation(listOfStations.at(indexOfStation).listOfKeys.at(indexOfKey).modeKey,
            listOfStations.at(indexOfStation).listOfKeys.at(indexOfKey).prioriryKey);

}

//save all stations and all groups to a file
void AppCore::saveListOfStationAndGroupsQJson(const QString &nameSave)
{
    QVariantMap all;

    QVariantList stations;
    //go over the list of stations
    for (const auto &listOfStation : listOfStations) {

        QVariantMap station;
        QVariantMap name;
        name.insert("nameOfStation", listOfStation.nameOfStation);
        name.insert("nameOfID", listOfStation.nameOfID);

        name.insert("nameIP", listOfStation.ip);
        name.insert("nameMaskNetwork", listOfStation.maskNetwork);
        name.insert("nameGateway", listOfStation.gateway);

        QVariantList keys;
        //go over the list of keys for the list of the station
        for (const auto &listOfKey : listOfStation.listOfKeys) {
            QVariantMap key;

            key.insert("uidOfKey", listOfKey.uidOfKey);
            key.insert("numberOfKey", listOfKey.numberOfKey);
            key.insert("nameOfKey", listOfKey.nameOfKey);
            key.insert("functionOfKey", listOfKey.functionOfKey);
            key.insert("assignedKey", listOfKey.assignedKey);
            key.insert("prioriryKey", listOfKey.prioriryKey);
            key.insert("modeKey", listOfKey.modeKey);

            keys << key;
        }

        station.insert("name", name);
        station.insert("keys", keys);

        stations << station;
    }

    QVariantList groups;
    //go over the list of stations
    for (const auto &listOfGroup : listOfGroups) {

        QVariantMap group;
        QVariantMap name;

        name.insert("nameOfGorup", listOfGroup.nameOfGorup);
        name.insert("nameOfID", listOfGroup.nameOfID);

        QVariantList stations;
        //go over the list of keys for the list of the station
        for (const auto &j : listOfGroup.stations) {
            QVariantMap station;

            station.insert("nameOfStation", j.nameOfStation);
            station.insert("nameOfID", j.nameOfID);
            stations << station;
        }

        group.insert("name", name);
        group.insert("stations", stations);

        groups << group;
    }

    all.insert("stationsAll", stations);
    all.insert("groupsAll", groups);

    QJson::Serializer serializer;
    bool ok;
    QByteArray json = serializer.serialize(all, &ok);
    if (ok) {
        qDebug() << json;

        //save JSON to a file
        QFile file(nameSave);
        file.open(QIODevice::WriteOnly);
        file.write(json);
        file.close();

        listOfFiles.push_front(nameSave);

        //save history to a file
        QFile fileList("history");
        fileList.open(QIODevice::WriteOnly);
        QString strFiles = listOfFiles.at(0) +  "\n" + listOfFiles.at(1) +  "\n" + listOfFiles.at(2)  +  "\n";
        QTextStream out(&fileList);
        out << strFiles ;
        fileList.close();

        emit historyOfFiles(listOfFiles.at(0), listOfFiles.at(1), listOfFiles.at(2));
    } else {
        qDebug() << "Something went wrong:" << serializer.errorMessage();
    }
}

//save all stations and all groups to a file
void AppCore::saveListOfStationAndGroupsQJson()
{
    QVariantMap all;

    QVariantList stations;
    //go over the list of stations
    for (const auto &listOfStation : listOfStations) {

        QVariantMap station;
        QVariantMap name;
        name.insert("nameOfStation", listOfStation.nameOfStation);
        name.insert("nameOfID", listOfStation.nameOfID);

        name.insert("nameIP", listOfStation.ip);
        name.insert("nameMaskNetwork", listOfStation.maskNetwork);
        name.insert("nameGateway", listOfStation.gateway);

        QVariantList keys;
        //go over the list of keys for the list of the station
        for (const auto &listOfKey : listOfStation.listOfKeys) {
            QVariantMap key;

            key.insert("uidOfKey", listOfKey.uidOfKey);
            key.insert("numberOfKey", listOfKey.numberOfKey);
            key.insert("nameOfKey", listOfKey.nameOfKey);
            key.insert("functionOfKey", listOfKey.functionOfKey);
            key.insert("assignedKey", listOfKey.assignedKey);
            key.insert("prioriryKey", listOfKey.prioriryKey);
            key.insert("modeKey", listOfKey.modeKey);

            keys << key;
        }

        station.insert("name", name);
        station.insert("keys", keys);

        stations << station;
    }

    QVariantList groups;
    //go over the list of stations
    for (const auto &listOfGroup : listOfGroups) {

        QVariantMap group;
        QVariantMap name;

        name.insert("nameOfGorup", listOfGroup.nameOfGorup);
        name.insert("nameOfID", listOfGroup.nameOfID);

        QVariantList stations;
        //go over the list of keys for the list of the station
        for (const auto &j : listOfGroup.stations) {
            QVariantMap station;

            station.insert("nameOfStation", j.nameOfStation);
            station.insert("nameOfID", j.nameOfID);
            stations << station;
        }

        group.insert("name", name);
        group.insert("stations", stations);

        groups << group;
    }

    all.insert("stationsAll", stations);
    all.insert("groupsAll", groups);

    QJson::Serializer serializer;
    bool ok;
    QByteArray json = serializer.serialize(all, &ok);
    if (ok) {
        qDebug() << json;

        if ((listOfFiles.at(0) != "") && (stateNewProejct == false)) {
            //save JSON to a file
            QFile file(listOfFiles.at(0));
            file.open(QIODevice::WriteOnly);
            file.write(json);
            file.close();
//Doesn't need
//			//save history to a file
//			QFile fileList("history");
//			fileList.open(QIODevice::WriteOnly);
//			QString strFiles = listOfFiles.at(0) +  "\n" + listOfFiles.at(1) +  "\n" + listOfFiles.at(2)  +  "\n";
//			QTextStream out(&fileList);
//			out << strFiles ;
//			fileList.close();
        } else {
            checkChanges(false);
        }

    } else {
        qCritical() << "Something went wrong:" << serializer.errorMessage();
    }
}
void AppCore::updateInfoAboutListFiles()
{
    emit historyOfFiles(listOfFiles.at(0), listOfFiles.at(1), listOfFiles.at(2));
}

//load all stations and all groups from a file
void AppCore::loadListOfStationAndGroupsQJson()
{
    QByteArray json;

    //load JSON from a file
    QFile file("configure.txt");
    file.open(QIODevice::ReadOnly);
    json = file.readAll();
    file.close();

    QList < ListOfStations> tmpListOfStations;
    ListOfStations tmpStation;
    QJson::Parser parser;
    bool ok;

    QVariantMap mapAll = parser.parse(json, &ok).toMap();

    QVariantList stations = mapAll["stationsAll"].toList();
    if (!ok) {
        qDebug() << "An error occurred during parsing";
    } else {
        for (const auto &station : stations) {
            QVariantMap mapStation = station.toMap();
            QVariantMap name = mapStation["name"].toMap();

            QString nameOfStation = name["nameOfStation"].toString();
            QString nameOfIDStation = name["nameOfID"].toString();

            QString nameIP = name["nameIP"].toString();
            QString nameMaskNetwork = name["nameMaskNetwork"].toString();
            QString nameGateway = name["nameGateway"].toString();

            QVariantList keys = mapStation["keys"].toList();

            tmpStation.nameOfStation = nameOfStation;
            tmpStation.nameOfID = nameOfIDStation;

            tmpStation.ip = nameIP;
            tmpStation.maskNetwork = nameMaskNetwork;
            tmpStation.gateway = nameGateway;

            tmpStation.listOfKeys.clear();

            for (const auto &key : keys) {
                ListOfkeys tmpKey;

                QVariantMap keyMap = key.toMap();

                QString uidOfKey = keyMap["uidOfKey"].toString();
                QString numberOfKey = keyMap["numberOfKey"].toString();
                QString nameOfKey = keyMap["nameOfKey"].toString();
                QString functionOfKey = keyMap["functionOfKey"].toString();
                QString assignedKey = keyMap["assignedKey"].toString();
                QString prioriryKey = keyMap["prioriryKey"].toString();
                QString modeKey = keyMap["modeKey"].toString();

                tmpKey.uidOfKey = uidOfKey;
                tmpKey.nameOfKey = nameOfKey;
                tmpKey.assignedKey = assignedKey;
                tmpKey.functionOfKey = functionOfKey;
                tmpKey.numberOfKey = numberOfKey;
                tmpKey.prioriryKey = prioriryKey;
                tmpKey.modeKey = modeKey;

                tmpStation.listOfKeys.append(tmpKey);
            }
            tmpListOfStations.append(tmpStation);
        }
        listOfStationsSaved = listOfStations = tmpListOfStations;

        if (listOfStations.size() > 0) {
            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list =  listOfStations.last().nameOfID.split(rx, QString::SkipEmptyParts);

            if (list.size() >= 2) {
                //            const QString &type = list.at(0);
                const QString &idStr = list.at(1);
                qint32 id = idStr.toInt();
                emit setCounterDigitalStation(id);
            }
        }

        emit sendClearAllListOfStations();

        //update a view of stations
        for (const auto &listOfStation : listOfStations) {
            //add new member to the list
            emit sendUpdateListOfStationsFromFile(listOfStation.nameOfStation, listOfStation.nameOfID);
        }
    }

    QList < ListOfGroups> tmpListOfGroups;
    ListOfGroups tmpGroup;

    QVariantList groups = mapAll["groupsAll"].toList();
    if (!ok) {
        qDebug() << "An error occurred during parsing";
    }

    for (const auto &group : groups) {
        QVariantMap mapGroup = group.toMap();
        QVariantMap name = mapGroup["name"].toMap();

        QString nameOfGorup = name["nameOfGorup"].toString();
        QString nameOfIDGroup = name["nameOfID"].toString();

        QVariantList stations = mapGroup["stations"].toList();

        tmpGroup.nameOfGorup = nameOfGorup;
        tmpGroup.nameOfID = nameOfIDGroup;
        tmpGroup.stations.clear();

        for (const auto &station : stations) {
            ListOfStations tmpStation;

            QVariantMap stationMap = station.toMap();

            QString nameOfStation = stationMap["nameOfStation"].toString();
            QString nameOfIDStation = stationMap["nameOfID"].toString();

            tmpStation.nameOfID = nameOfIDStation;
            tmpStation.nameOfStation = nameOfStation;

            tmpGroup.stations.append(tmpStation);
        }
        tmpListOfGroups.append(tmpGroup);
    }
    listOfGroupsSaved = listOfGroups = tmpListOfGroups;

    if (listOfGroups.size() > 0) {
        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroups.last().nameOfID.split(rx, QString::SkipEmptyParts);

        if (list.size() >= 2) {
            //            const QString &type = list.at(0);
            const QString &idStr = list.at(1);
            qint32 id = idStr.toInt();
            emit setCounterGroup(id);
        }
    }

    //clear current list of groups
    emit sendClearAllListOfGroups();

    //update a view of groups
    for (const auto &listOfGroup : listOfGroups) {
        //add new member to the list
        emit sendUpdateListOfGroupsFromFile(listOfGroup.nameOfGorup, listOfGroup.nameOfID);
    }

    stateNewProejct = false;
//    listOfGroupsSaved = listOfGroups;
//    listOfStationsSaved = listOfStations;
}

//load all stations and all groups from a file
void AppCore::loadListOfStationAndGroupsQJson(const QString &nameLoad)
{
    QByteArray json;

    //load JSON from a file
    QFile file(nameLoad);
    file.open(QIODevice::ReadOnly);
    json = file.readAll();
    file.close();

//    //add loaded file
//    listOfFiles.push_front(nameLoad);

//    //save history to a file
//    QFile fileList("history");
//    fileList.open(QIODevice::WriteOnly);
//    QString strFiles = listOfFiles.at(0) +  "\n" + listOfFiles.at(1) +  "\n" + listOfFiles.at(2)  +  "\n";
//    QTextStream out(&fileList);
//    out << strFiles ;
//    fileList.close();

//    emit historyOfFiles(listOfFiles.at(0), listOfFiles.at(1), listOfFiles.at(2));


    QList < ListOfStations> tmpListOfStations;
    ListOfStations tmpStation;
    QJson::Parser parser;
    bool ok;

    QVariantMap mapAll = parser.parse(json, &ok).toMap();

    QVariantList stations = mapAll["stationsAll"].toList();
    if (!ok) {
        qDebug() << "An error occurred during parsing";
    } else {
        for (const auto &station : stations) {
            QVariantMap mapStation = station.toMap();
            QVariantMap name = mapStation["name"].toMap();

            QString nameOfStation = name["nameOfStation"].toString();
            QString nameOfIDStation = name["nameOfID"].toString();

            QString nameIP = name["nameIP"].toString();
            QString nameMaskNetwork = name["nameMaskNetwork"].toString();
            QString nameGateway = name["nameGateway"].toString();

            QVariantList keys = mapStation["keys"].toList();

            tmpStation.nameOfStation = nameOfStation;
            tmpStation.nameOfID = nameOfIDStation;

            tmpStation.ip = nameIP;
            tmpStation.maskNetwork = nameMaskNetwork;
            tmpStation.gateway = nameGateway;

            tmpStation.listOfKeys.clear();

            for (const auto &key : keys) {
                ListOfkeys tmpKey;

                QVariantMap keyMap = key.toMap();

                QString uidOfKey = keyMap["uidOfKey"].toString();
                QString numberOfKey = keyMap["numberOfKey"].toString();
                QString nameOfKey = keyMap["nameOfKey"].toString();
                QString functionOfKey = keyMap["functionOfKey"].toString();
                QString assignedKey = keyMap["assignedKey"].toString();
                QString prioriryKey = keyMap["prioriryKey"].toString();
                QString modeKey = keyMap["modeKey"].toString();

                tmpKey.uidOfKey = uidOfKey;
                tmpKey.nameOfKey = nameOfKey;
                tmpKey.assignedKey = assignedKey;
                tmpKey.functionOfKey = functionOfKey;
                tmpKey.numberOfKey = numberOfKey;
                tmpKey.prioriryKey = prioriryKey;
                tmpKey.modeKey = modeKey;

                tmpStation.listOfKeys.append(tmpKey);
            }
            tmpListOfStations.append(tmpStation);
        }
        listOfStationsSaved = listOfStations = tmpListOfStations;

        if (listOfStations.size() > 0) {
            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list =  listOfStations.last().nameOfID.split(rx, QString::SkipEmptyParts);

            if (list.size() >= 2) {
                const QString &idStr = list.at(1);
                qint32 id = idStr.toInt();
                emit setCounterDigitalStation(id);
            }
        }

        emit sendClearAllListOfStations();

        //update a view of stations
        for (const auto &listOfStation : listOfStations) {
            //add new member to the list
            emit sendUpdateListOfStationsFromFile(listOfStation.nameOfStation, listOfStation.nameOfID);
        }
    }

    QList < ListOfGroups> tmpListOfGroups;
    ListOfGroups tmpGroup;

    QVariantList groups = mapAll["groupsAll"].toList();
    if (!ok) {
        qDebug() << "An error occurred during parsing";
    }

    for (const auto &group : groups) {
        QVariantMap mapGroup = group.toMap();
        QVariantMap name = mapGroup["name"].toMap();

        QString nameOfGorup = name["nameOfGorup"].toString();
        QString nameOfIDGroup = name["nameOfID"].toString();

        QVariantList stations = mapGroup["stations"].toList();

        tmpGroup.nameOfGorup = nameOfGorup;
        tmpGroup.nameOfID = nameOfIDGroup;
        tmpGroup.stations.clear();

        for (const auto &station : stations) {
            ListOfStations tmpStation;

            QVariantMap stationMap = station.toMap();

            QString nameOfStation = stationMap["nameOfStation"].toString();
            QString nameOfIDStation = stationMap["nameOfID"].toString();

            tmpStation.nameOfID = nameOfIDStation;
            tmpStation.nameOfStation = nameOfStation;

            tmpGroup.stations.append(tmpStation);
        }
        tmpListOfGroups.append(tmpGroup);
    }
    listOfGroupsSaved = listOfGroups = tmpListOfGroups;

    if (listOfGroups.size() > 0) {
        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroups.last().nameOfID.split(rx, QString::SkipEmptyParts);

        if (list.size() >= 2) {
            //            const QString &type = list.at(0);
            const QString &idStr = list.at(1);
            qint32 id = idStr.toInt();
            emit setCounterGroup(id);
        }
    }

    //clear current list of groups
    emit sendClearAllListOfGroups();

    //update a view of groups
    for (const auto &listOfGroup : listOfGroups) {
        //add new member to the list
        emit sendUpdateListOfGroupsFromFile(listOfGroup.nameOfGorup, listOfGroup.nameOfID);
    }


    stateNewProejct = false;

    if (nameLoad != listOfFiles.at(0)) {
        listOfFiles.push_front(nameLoad);

        //save history to a file
        QFile fileList("history");
        fileList.open(QIODevice::WriteOnly);
        QString strFiles = listOfFiles.at(0) +  "\n" + listOfFiles.at(1) +  "\n" + listOfFiles.at(2)  +  "\n";
        QTextStream out(&fileList);
        out << strFiles ;
        fileList.close();

        emit historyOfFiles(listOfFiles.at(0), listOfFiles.at(1), listOfFiles.at(2));
    }

//    listOfGroupsSaved = listOfGroups;
//    listOfStationsSaved = listOfStations;

}

void AppCore::updateAllViews()
{

    emit sendClearAllListOfStations();

    //update a view of stations
    for (const auto &listOfStation : listOfStations) {
        //add new member to the list
        emit sendUpdateListOfStationsFromFile(listOfStation.nameOfStation, listOfStation.nameOfID);
    }

    //clear current list of groups
    emit sendClearAllListOfGroups();

    //update a view of groups
    for (const auto &listOfGroup : listOfGroups) {
        //add new member to the list
        emit sendUpdateListOfGroupsFromFile(listOfGroup.nameOfGorup, listOfGroup.nameOfID);
    }
}

void AppCore::sendPositionInListOfStation()
{
    emit getCurrentPositionListOfStation();
}

void AppCore::updateAllViewsWithPostion()
{
    updateAllViews();
    //send restored position
    emit restoreCurrentPositionListOfStation();
}

void AppCore::updateStationName(const QString &currentNameOfStation, const QString &newNameOfStation)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == currentNameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //update for new name of station
    listOfStations[indexOfStation].nameOfStation = newNameOfStation;
}

void AppCore::updateStationId(const QString &currentNameOfStation, const QString &newNameOfId)
{

    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == currentNameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //update for new number of station
    listOfStations[indexOfStation].nameOfID = newNameOfId;
}

void AppCore::updateStationIp(const QString &currentNameOfStation, const QString &newIP)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == currentNameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //update for new ip of station
    listOfStations[indexOfStation].ip = newIP;
}

void AppCore::updateStationMaskNetwork(const QString &currentNameOfStation, const QString &newMaskNetwork)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == currentNameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //update for new mask of station
    listOfStations[indexOfStation].maskNetwork = newMaskNetwork;
}

void AppCore::updateStationGateway(const QString &currentNameOfStation, const QString &newGateway)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == currentNameOfStation) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //update for new getaway of station
    listOfStations[indexOfStation].gateway = newGateway;
}
