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

void AppCore::receiveInformationAboutPort()
{
    listUarts.clear();
    if (port.isOpen()) {
        QString temp = uartDev + QString(" ") + QString::number(uartSpeed) + QString("-8-N-1") ;
        emit sendToQmlPortOpenedPort(temp);
    }
}

//close port
void  AppCore::receiveFromQmlClosePort()
{
    if (port.isOpen()) {
        port.close();
        QString temp  = "";
        emit sendToQmlPortOpenedPort(temp);
    }
}

// transfer device /dev/ttyUSBx and speed
void AppCore::receiveFromQmlOpenPort(const QString &dev, qint32 speed)
{
    if (!port.isOpen()) {
        port.setPortName(dev);
        if (
            port.setBaudRate(speed) &&
            port.setDataBits(QSerialPort::Data8) &&
            port.setParity(QSerialPort::NoParity) &&
            port.setStopBits(QSerialPort::OneStop) &&
            port.setFlowControl(QSerialPort::NoFlowControl)) {
            if (port.open((QIODevice::ReadWrite))) {
                uartDev = dev;
                uartSpeed = speed;
                QString temp =  uartDev + QString(" ") + QString::number(uartSpeed) + QString("-8-N-1") ;
                emit sendToQmlPortOpenedPort(temp);
            } else {
                qDebug() << "Open error";
            }
        }
    }
}

//update list all ports
void AppCore::updateListOfUsbUarts()
{
    QList <QString> currentListUarts;

    // update existing ports
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        portNameUarts = serialPortInfo.portName();
        currentListUarts.append(portNameUarts);
    }

    // if there is no element then add
    foreach (QString currentPorts, currentListUarts) {
        if (listUarts.indexOf(currentPorts) == -1) {
            listUarts.append(currentPorts);
            emit sendToQmlPortSerial(currentPorts);
        }
    }

    // search for nonexistent elements
    if (!currentListUarts.isEmpty()) {
        foreach (QString deletePorts, listUarts) {
            //foreach (QString currentPorts, currentListUarts) {
            if (currentListUarts.indexOf(deletePorts) == -1) {
                // delete deletePorts its index
                int index = listUarts.indexOf(deletePorts);
                emit sendToQmlPortSerialDeleteIndex(index);
                listUarts.removeAll(deletePorts);
            }
            //}
        }
    } else {
        // delete all elements
        emit sendToQmlPortSerialDeleteAll();
    }
}

//send the full configuration to the board
void AppCore::sendConfigJsonToStation(const QString &stationID)
{

    QVariantMap all;

    QVariantList stations;

    QRegExp rx("[, ]"); // match a comma or a space
    QStringList list =  stationID.split(rx, QString::SkipEmptyParts);

    if (!(list.size() >= 2)) {
        qCritical() << "Something went wrong, own ID fail";
        return;
    }

    all.insert("ownId", list.at(1).toInt());

    //go over the list of stations
    for (const auto &listOfStation : listOfStations) {

        QVariantMap station;
        QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfStation.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << "Something went wrong, a name ID for stations";
            return;
        }

        //const QString& type = list.at(0);
        const QString &idStr = list.at(1);

        qint32 id = idStr.toInt();
        name.insert("id", id);

        name.insert("ip", listOfStation.ip);
        name.insert("mask", listOfStation.maskNetwork);
        name.insert("gateway", listOfStation.gateway);

        QVariantList keys;
        //go over the list of keys for the list of the station
        for (const auto &listOfKey : listOfStation.listOfKeys) {
            QVariantMap key;

            qint32 numberKey = listOfKey.numberOfKey.toInt();
            key.insert("key", numberKey);

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

            qint32 typeFunction = compareString(listOfKey.functionOfKey);
            if (typeFunction == 0) {
                qCritical() << "Something went wrong, a function for keys";
                return;
            }

            if (typeFunction == 1) {
                //duplex
                key.insert("link", 2);
                key.insert("function", 1);
            } else if (typeFunction == 6) {
                //simplex
                key.insert("link", 1);
                key.insert("function", 1);
            } else {
                key.insert("link", 0);
                key.insert("function", typeFunction);
            }

            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list = listOfKey.assignedKey.split(rx, QString::SkipEmptyParts);

            if ((!(list.size() >= 2)) && ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7))) {
                qCritical() << "Something went wrong, CID ID for stations";
                return;
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
                    //3, 4, 5, 7
                    if ((typeForAssign == -1) && (typeFunction != 7) && (typeFunction != 5) && (typeFunction != 4) && (typeFunction != 3)) {
                        qCritical() << "Something went wrong, type for assign";
                        return;
                    }
                    key.insert("type", typeForAssign);
                    //id of groups or stations
                    qint32 id = idStr.toInt();
                    key.insert("assign", id);
                } else {
                    key.insert("type", 0);
                    key.insert("assign", 0);
                }
            }

            qint32 modeKey = listOfKey.modeKey.toInt();
            key.insert("mode", modeKey);

            qint32 prioriryKey = listOfKey.prioriryKey.toInt();
            key.insert("prioriry", prioriryKey);

            keys << key;
        }

        station.insert("name", name);
        station.insert("keys", keys);

        stations << station;
    }

    QVariantList groups;
    //go over the list of groups
    for (const auto &listOfGroup : listOfGroups) {

        QVariantMap group;
        QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroup.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << "Something went wrong, a name ID for groups";
            return;
        }

        name.insert("number", list.at(1).toInt());

        QVariantList stations;
        //go over the list of keys for the list of the station
        for (const auto &j : listOfGroup.stations) {
            QVariantMap station;

            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list =  j.nameOfID.split(rx, QString::SkipEmptyParts);

            if (!(list.size() >= 2)) {
                qCritical() << "Something went wrong, a name ID for stations in groups";
                return;
            }

            station.insert("id", list.at(1).toInt());
            stations << station;
        }

        group.insert("name", name);
        group.insert("stations", stations);

        groups << group;
    }

    all.insert("stationsAll", stations);
    all.insert("groupsAll", groups);

    if(!startVerificationStations()) {
        return;
    }

    //sending data to uart
    QJson::Serializer serializer;
    bool ok;
    QByteArray json = serializer.serialize(all, &ok);
    if (ok) {
        qDebug() << json;

        //max size of step for transferring
        constexpr qint32 SIZE_DEF = 128;
        constexpr qint32 SIZE_MAX_SIZE_READ = 256;
        constexpr qint32 TIME_OUT = 2000;

        if (port.isOpen()) {

            //write data
            qint32 sizeBuf = json.size();
            qint32 allPackegs = static_cast<int>(ceil(static_cast<double>(sizeBuf) / static_cast<double>(SIZE_DEF))) - 1;
            qint32 counterPack = 0;

            for (qint32 i = 0; i < sizeBuf; i += SIZE_DEF) {
                QByteArray tmp;

                for (qint32 j = i; j < (i < sizeBuf - SIZE_DEF ? i + SIZE_DEF : sizeBuf); j++) {
                    tmp.append(json.at(j));
                }

                QVariantMap cmd;
                QRegExp rx("[, ]"); // match a comma or a space
                QStringList list =  stationID.split(rx, QString::SkipEmptyParts);

                const QString &idStr = list.at(1);
                qint32 id = idStr.toInt();
                cmd.insert("writeConfigId", id);
                cmd.insert("number", counterPack);
                cmd.insert("all", allPackegs);
                cmd.insert("size", tmp.size());
                cmd.insert("config", tmp);

                QJson::Serializer serializerSend;
                bool okSend;
                QByteArray jsonSend = serializerSend.serialize(cmd, &okSend);

                int fillNull =  256 - jsonSend.size();
                //Add 0
                for (int i = 0; i < fillNull; i++) {
                    jsonSend.append(static_cast<char>(0));
                }

                if (okSend) {
                    qDebug() << jsonSend;

                    const qint64 bytesWritten = port.write(jsonSend);
                    port.waitForBytesWritten(TIME_OUT);

                    if (bytesWritten == -1) {
                        qDebug() << __LINE__ <<__FUNCTION__;
                    } else if (bytesWritten != jsonSend.size()) {
                        qDebug() << __LINE__ << __FUNCTION__;
                    } else if (!port.waitForBytesWritten(TIME_OUT)) {
                        qDebug() << __LINE__ << __FUNCTION__;
                    }

                    if (counterPack == allPackegs) {

                        QByteArray tmpBytes;

                        tmpBytes = port.readAll();
                        while (port.waitForReadyRead(10000))
                            tmpBytes.append(port.readAll());

                        QJson::Parser parser;
                        bool okRead;

                        QVariantMap result = parser.parse(tmpBytes, &okRead).toMap();
                        if (okRead) {

                            qint32 writeConfigId = result["writeConfigId"].toInt();
                            QString status = QString(result["status"].toString());

                            if ((writeConfigId >= MIN_ID) && (status == QString("ok"))) {
                                emit createWindowStatus(stationID, QString("Ok"));
                            }
                        }
                    }
                    counterPack++;
                }

                //clear the temp buffer
                tmp.clear();
            }
        }
    } else {
        qCritical() << "Something went wrong:" << serializer.errorMessage();
    }
}
