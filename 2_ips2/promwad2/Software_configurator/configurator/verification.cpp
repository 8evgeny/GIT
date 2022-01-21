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
#include <QHash>

#include <string>
#include <functional>
#include <QHostAddress>

bool AppCore::ipVerification(const QString &ip)
{
    QHostAddress addressIp(ip);
    return QAbstractSocket::IPv4Protocol == addressIp.protocol();
}

bool AppCore::keyVerification(const QString &stationName, const QString &keyNumber)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    if (keyNumber == "")
        return false;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == stationName) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    //there is no such station in the list
    if (isStation) {
        uint32_t comp = 0;
        for (const auto &listOfKey : listOfStations.at(indexOfStation).listOfKeys) {
            if (keyNumber == listOfKey.numberOfKey) {
                comp++;
            }
        }
        if (comp > 1) {
            return false;
        }
    }

    return (keyNumber.toInt() >= MIN_KEY_NUMBER) && (keyNumber.toInt() <= MAX_KEY_NUMBER);
}

bool AppCore::functionVerification(const QString &functionName)
{

    auto compareString = [](const QString & x) {
        if (x == QString("1"))
            return directConnectionDuplex;

        if (x == QString("2"))
            return groupCommunication;

        if (x == QString("3"))
            return circularBond;

        if (x == QString("4"))
            return conferenceCall;

        if (x == QString("5"))
            return telephoneCommunications;

        if (x == QString("6"))
            return directConnectionSimplex;

        if (x == QString("7"))
            return hangUp;

        return noneConnection;
    };

    return compareString(functionName) != noneConnection;
}

bool AppCore::startVerificationStations()
{
    //QVariantMap all;
    QString errors;
    QTextStream outErrors(&errors);

    //QVariantList stations;

    //go over the list of stations
    for (const auto &listOfStation : listOfStations) {

        //QVariantMap station;
        //QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfStation.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << __LINE__ << " - " << "Something went wrong, a name ID for stations";
            outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a name ID for stations" << "\n";

            emit sendWrongGeneric();
            emit sendWrongNameId(listOfStation.nameOfStation);

        } else {
            const QString &type = list.at(0);
            const QString &idStr = list.at(1);
            qint32 typeForAssign = -1;

            //digital stations
            if (type == QString("CID"))
                typeForAssign = 0;

            //groups
            if (type == QString("GR"))
                typeForAssign = 1;

            if (typeForAssign == -1) {
                qCritical() << __LINE__ << " - " << "Something went wrong, type for assign";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, type for assign" << "\n";
                //emit to ui
                emit sendWrongGeneric();
                emit sendWrongTypeAssign(listOfStation.nameOfStation);
            }

            qint32 id = idStr.toInt();
            if (!((id >= MIN_ID) && (id <= MAX_ID))) {
                qCritical() << __LINE__ << "Something went wrong, a Id of this key is not valid";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a Id of this key is not valid" << "\n";

                emit sendWrongGeneric();
                emit sendWrongIdKeyNotValid(listOfStation.nameOfStation);
            }
        }

        //verification of Ip, Mask and Gateway
        QHostAddress addressIp(listOfStation.ip);
        QHostAddress addressMask(listOfStation.maskNetwork);
        QHostAddress addressGateway(listOfStation.gateway);

        if (!(QAbstractSocket::IPv4Protocol == addressIp.protocol())) {
            qCritical() << __LINE__ << " - " << "Something went wrong, IP is not valid";
            outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, IP is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongIp(listOfStation.nameOfStation);
        }

        if (!(QAbstractSocket::IPv4Protocol == addressMask.protocol())) {
            qCritical() << __LINE__ << " - " << "Something went wrong, Mask is not valid";
            outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, Mask is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongMask(listOfStation.nameOfStation);
        }

        if (!(QAbstractSocket::IPv4Protocol == addressGateway.protocol())) {
            qCritical() << __LINE__ << " - " << "Something went wrong, Gateway is not valid";
            outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, Gateway is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongGateway(listOfStation.nameOfStation);
        }

        //QVariantList keys;
        //go over the list of keys for the list of the station
        for (int j = 0; j < listOfStation.listOfKeys.size(); j++) {

            qint32 numberKey = listOfStation.listOfKeys.at(j).numberOfKey.toInt();
            if (!((numberKey >= MIN_KEY_NUMBER) && (numberKey <= MAX_KEY_NUMBER))) {
                qCritical() << __LINE__ << " - " << "Something went wrong, a number of this key is not valid";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a number of this key is not valid" << "\n";

                emit sendWrongGeneric();
                emit sendWrongNumberKeyNotValid(listOfStation.nameOfStation);
            }

            auto compareString = [](const QString & x) {
                if (x == QString("1"))
                    return directConnectionDuplex;

                if (x == QString("2"))
                    return groupCommunication;

                if (x == QString("3"))
                    return circularBond;

                if (x == QString("4"))
                    return conferenceCall;

                if (x == QString("5"))
                    return telephoneCommunications;

                if (x == QString("6"))
                    return directConnectionSimplex;

                if (x == QString("7"))
                    return hangUp;

                return noneConnection;
            };

            qint32 typeFunction = compareString(listOfStation.listOfKeys.at(j).functionOfKey);

            if (typeFunction == 0) {
                qCritical() << __LINE__ << " - " << "Something went wrong, a function for keys";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a function for keys" << "\n";

                emit sendWrongGeneric();
                emit sendWrongFunctionKeys(listOfStation.nameOfStation);
            }

            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list = listOfStation.listOfKeys.at(j).assignedKey.split(rx, QString::SkipEmptyParts);

            if ((!(list.size() >= 2)) && ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7))) {
                qCritical() << __LINE__ << " - " << "Something went wrong, CID ID for stations";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, CID ID for stations" << "\n";

                emit sendWrongGeneric();
                emit sendWrongCidId(listOfStation.nameOfStation);
            } else {
                if ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7)) {
                    const QString &type = list.at(0);
                    const QString &idStr = list.at(1);

                    qint32 typeForAssign = -1;

                    //digital stations
                    if (type == QString("CID"))
                        typeForAssign = 0;

                    //groups
                    if (type == QString("GR"))
                        typeForAssign = 1;

                    if ((typeForAssign == -1) && (typeFunction != 7) && (typeFunction != 5) && (typeFunction != 4) && (typeFunction != 3)) {
                        qCritical() << "Something went wrong, type for assign";
                        outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, type for assign" << "\n";

                        emit sendWrongGeneric();
                        emit sendWrongTypeAssign(listOfStation.nameOfStation);
                    }

                    //id of groups or stations
                    qint32 id = idStr.toInt(); // it is only for stations
                    if (typeForAssign == 0) {
                        if (!((id >= MIN_ID) && (id <= MAX_ID))) {
                            qCritical() << __LINE__ << "Something went wrong, a Id of this key is not valid";
                            outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - "  << "Something went wrong, a Id of this key is not valid" << "\n";

                            emit sendWrongGeneric();
                            emit sendWrongIdKeyNotValid(listOfStation.nameOfStation);
                        }
                    }
                }
            }

            qint32 modeKey = listOfStation.listOfKeys.at(j).modeKey.toInt();
            if (!((modeKey >= MIN_MOD_KEY) && (modeKey <= MAX_MOD_KEY))) {
                qCritical() << __LINE__ << "Something went wrong, a Mode of this key is not valid";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a Mode of this key is not valid" << "\n";

                emit sendWrongGeneric();
                emit sendWrongMode(listOfStation.nameOfStation);
            }

            qint32 priorityKey = listOfStation.listOfKeys.at(j).prioriryKey.toInt();
            if (!((priorityKey >= MIN_PRIORITY_KEY) && (priorityKey <= MAX_PRIORITY_KEY))) {
                qCritical() << __LINE__ << "Something went wrong, a Priority of this key is not valid";
                outErrors << listOfStation.nameOfStation << " " << listOfStation.nameOfID << " - " << "Something went wrong, a Priority of this key is not valid" << "\n";

                emit sendWrongGeneric();
                emit sendWrongPriority(listOfStation.nameOfStation);
            }
        }
    }
    QString textErrors;
    textErrors =  outErrors.readAll();

    if ((textErrors == "") && startVerificationGroups()) {
        emit sendOkGeneric();
        return true;
    }
    return false;
}

bool AppCore::startVerificationGroups()
{
    //QVariantMap all;
    QString errors;
    QTextStream outErrors(&errors);

    //go over the list of stations
    for (const auto &listOfGroup : listOfGroups) {


        //QVariantMap station;
        //QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroup.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << __LINE__ << " - " << "Something went wrong, a name ID for groups";
            outErrors << listOfGroup.nameOfGorup << " " << listOfGroup.nameOfID << " - " << "Something went wrong, a name ID for groups" << "\n";

            emit sendWrongGeneric();
            emit sendWrongNameId(listOfGroup.nameOfGorup);
        } else {
            const QString &type = list.at(0);
            const QString &idStr = list.at(1);

            qint32 typeForAssign = -1;

            //digital stations
            if (type == QString("GR"))
                typeForAssign = 1;

            //id of groups or stations
            qint32 id = idStr.toInt(); // for groups

            if (typeForAssign != 1) {
                qCritical() << __LINE__ << " - " << "Something went wrong, type for groups";
                outErrors << listOfGroup.nameOfGorup << " " << listOfGroup.nameOfID << " - " << "Something went wrong, type for groups" << "\n";
                emit sendWrongGeneric();
                emit sendWrongNameInGroupsId(listOfGroup.nameOfGorup);
            }
            if (id < 0) {
                qCritical() << __LINE__ << " - " << "Something went wrong, id of the group error";
                outErrors << listOfGroup.nameOfGorup << " " << listOfGroup.nameOfID << " - " << "Something went wrong, id of the group error" << "\n";
                emit sendWrongGeneric();
                emit sendWrongNameInGroupsId(listOfGroup.nameOfGorup);
            }

            //look for errors (if there is no such station)
            for (int i = 0; i < listOfGroup.stations.size(); i++) {

                QList<QString> list;
                for (const auto &listOfStation : listOfStations) {
                    list.append(listOfStation.nameOfStation);
                }

                QList<QString> listCid;
                for (const auto &listOfStationCid : listOfStations) {
                    listCid.append(listOfStationCid.nameOfID);
                }

                // search for element and if found change data
                QString str = listOfGroup.stations.at(i).nameOfStation;
                int k = list.indexOf(str);

                // search for element and if found change data
                QString strCid = listOfGroup.stations.at(i).nameOfID;
                int kCid = listCid.indexOf(strCid);

                if ((k != -1) && (kCid != -1)) {
                    // ...
                } else {
                    qCritical() << __LINE__ << " - " << "Something went wrong, error from stations";
                    outErrors << listOfGroup.nameOfGorup << " " << listOfGroup.nameOfID << " - " << "Something went wrong, error from stations" << "\n";
                    emit sendWrongGeneric();
                    emit sendWrongNameInGroupsId(listOfGroup.nameOfGorup);
                }
            }
        }
    }

    QString textErrors;
    textErrors =  outErrors.readAll();
    if (textErrors == "") {
        return true;
    }

    return false;
}

void AppCore::startVerificationStation()
{
    emit getStartVerificationStation();
}

void AppCore::startVerificationStation(const QString &name)
{
    //for saving an index
    int indexOfStation = 0;
    bool isStation = false;

    if (name == "")
        return;

    //look for an index of the station
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == name) {
            indexOfStation = i;
            isStation = true;
            break;
        }
    }

    if (!isStation) {
        //there is no such station in the list
        return;
    }

    //QVariantMap all;
    QString errors;
    QTextStream outErrors(&errors);

    //QVariantList stations;

    //go over the list of stations
//    for (const auto &listOfStation : listOfStations) {

    //QVariantMap station;
    //QVariantMap name;

    QRegExp rx("[, ]"); // match a comma or a space
    QStringList list =  listOfStations.at(indexOfStation).nameOfID.split(rx, QString::SkipEmptyParts);

    if (!(list.size() >= 2)) {
        qCritical() << __LINE__ << " - " << "Something went wrong, a name ID for stations";
        outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a name ID for stations" << "\n";

        emit sendWrongGeneric();
        emit sendWrongNameId(listOfStations.at(indexOfStation).nameOfStation);

    } else {
        const QString &type = list.at(0);
        const QString &idStr = list.at(1);
        qint32 typeForAssign = -1;

        //digital stations
        if (type == QString("CID"))
            typeForAssign = 0;

        //groups
        if (type == QString("GR"))
            typeForAssign = 1;

        if (typeForAssign == -1) {
            qCritical() << __LINE__ << " - " << "Something went wrong, type for assign";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, type for assign" << "\n";
            //emit to ui
            emit sendWrongGeneric();
            emit sendWrongTypeAssign(listOfStations.at(indexOfStation).nameOfStation);
        }

        qint32 id = idStr.toInt();
        if (!((id >= MIN_ID) && (id <= MAX_ID))) {
            qCritical() << __LINE__ << "Something went wrong, a Id of this key is not valid";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a Id of this key is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongIdKeyNotValid(listOfStations.at(indexOfStation).nameOfStation);
        }
    }

    //verification of Ip, Mask and Gateway
    QHostAddress addressIp(listOfStations.at(indexOfStation).ip);
    QHostAddress addressMask(listOfStations.at(indexOfStation).maskNetwork);
    QHostAddress addressGateway(listOfStations.at(indexOfStation).gateway);

    if (!(QAbstractSocket::IPv4Protocol == addressIp.protocol())) {
        qCritical() << __LINE__ << " - " << "Something went wrong, IP is not valid";
        outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, IP is not valid" << "\n";

        emit sendWrongGeneric();
        emit sendWrongIp(listOfStations.at(indexOfStation).nameOfStation);
    }

    if (!(QAbstractSocket::IPv4Protocol == addressMask.protocol())) {
        qCritical() << __LINE__ << " - " << "Something went wrong, Mask is not valid";
        outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, Mask is not valid" << "\n";

        emit sendWrongGeneric();
        emit sendWrongMask(listOfStations.at(indexOfStation).nameOfStation);
    }

    if (!(QAbstractSocket::IPv4Protocol == addressGateway.protocol())) {
        qCritical() << __LINE__ << " - " << "Something went wrong, Gateway is not valid";
        outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, Gateway is not valid" << "\n";

        emit sendWrongGeneric();
        emit sendWrongGateway(listOfStations.at(indexOfStation).nameOfStation);
    }

    //QVariantList keys;
    //go over the list of keys for the list of the station
    for (int j = 0; j < listOfStations.at(indexOfStation).listOfKeys.size(); j++) {

        qint32 numberKey = listOfStations.at(indexOfStation).listOfKeys.at(j).numberOfKey.toInt();
        if (!((numberKey >= MIN_KEY_NUMBER) && (numberKey <= MAX_KEY_NUMBER))) {
            qCritical() << __LINE__ << " - " << "Something went wrong, a number of this key is not valid";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a number of this key is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongNumberKeyNotValid(listOfStations.at(indexOfStation).nameOfStation);
        }

        auto compareString = [](const QString & x) {
            if (x == QString("1"))
                return directConnectionDuplex;

            if (x == QString("2"))
                return groupCommunication;

            if (x == QString("3"))
                return circularBond;

            if (x == QString("4"))
                return conferenceCall;

            if (x == QString("5"))
                return telephoneCommunications;

            if (x == QString("6"))
                return directConnectionSimplex;

            if (x == QString("7"))
                return hangUp;

            return noneConnection;
        };

        qint32 typeFunction = compareString(listOfStations.at(indexOfStation).listOfKeys.at(j).functionOfKey);

        if (typeFunction == 0) {
            qCritical() << __LINE__ << " - " << "Something went wrong, a function for keys";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a function for keys" << "\n";

            emit sendWrongGeneric();
            emit sendWrongFunctionKeys(listOfStations.at(indexOfStation).nameOfStation);
        }

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list = listOfStations.at(indexOfStation).listOfKeys.at(j).assignedKey.split(rx, QString::SkipEmptyParts);

        if ((!(list.size() >= 2)) && ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7))) {
            qCritical() << __LINE__ << " - " << "Something went wrong, CID ID for stations";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, CID ID for stations" << "\n";

            emit sendWrongGeneric();
            emit sendWrongCidId(listOfStations.at(indexOfStation).nameOfStation);
        } else {
            if ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7)) {
                const QString &type = list.at(0);
                const QString &idStr = list.at(1);

                qint32 typeForAssign = -1;

                //digital stations
                if (type == QString("CID"))
                    typeForAssign = 0;

                //groups
                if (type == QString("GR"))
                    typeForAssign = 1;

                if (typeForAssign == -1) {
                    qCritical() << "Something went wrong, type for assign";
                    outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, type for assign" << "\n";

                    emit sendWrongGeneric();
                    emit sendWrongTypeAssign(listOfStations.at(indexOfStation).nameOfStation);
                }

                //id of groups or stations
                qint32 id = idStr.toInt(); // it is only for stations
                if (typeForAssign == 0) {
                    if (!((id >= MIN_ID) && (id <= MAX_ID))) {
                        qCritical() << __LINE__ << "Something went wrong, a Id of this key is not valid";
                        outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - "  << "Something went wrong, a Id of this key is not valid" << "\n";

                        emit sendWrongGeneric();
                        emit sendWrongIdKeyNotValid(listOfStations.at(indexOfStation).nameOfStation);
                    }
                }
            }
        }

        qint32 modeKey = listOfStations.at(indexOfStation).listOfKeys.at(j).modeKey.toInt();
        if (!((modeKey >= MIN_MOD_KEY) && (modeKey <= MAX_MOD_KEY))) {
            qCritical() << __LINE__ << "Something went wrong, a Mode of this key is not valid";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a Mode of this key is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongMode(listOfStations.at(indexOfStation).nameOfStation);
        }

        qint32 priorityKey = listOfStations.at(indexOfStation).listOfKeys.at(j).prioriryKey.toInt();
        if (!((priorityKey >= MIN_PRIORITY_KEY) && (priorityKey <= MAX_PRIORITY_KEY))) {
            qCritical() << __LINE__ << "Something went wrong, a Priority of this key is not valid";
            outErrors << listOfStations.at(indexOfStation).nameOfStation << " " << listOfStations.at(indexOfStation).nameOfID << " - " << "Something went wrong, a Priority of this key is not valid" << "\n";

            emit sendWrongGeneric();
            emit sendWrongPriority(listOfStations.at(indexOfStation).nameOfStation);
        }
    }
//    }

    QString textErrors;
    textErrors =  outErrors.readAll();
    if ((textErrors == "") && startVerificationGroups()) {
        emit sendOkGeneric();
    }
}
