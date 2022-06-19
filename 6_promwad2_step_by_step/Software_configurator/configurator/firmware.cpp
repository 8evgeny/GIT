/*!
 \file firmware.cpp

*/

#include "appcore.h"

#include <QFile>
#include "qaesencryption.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QDataStream>

#include <sstream>
#include <bitset>
#include <iomanip>

#include "Lib/qjson/include/QJson/Parser"
#include "Lib/qjson/include/QJson/Serializer"
#include "Lib/qjson/src/cmdlineparser.h"
#include "Lib/qjson/src/qjson_debug.h"

std::string AppCore::hexStr(const QByteArray &data)
{
    std::stringstream ss;
    ss << std::hex;
    for (char i : data) {
//        char str[3] = {0};
//        sprintf(str, "%02x", );
        auto c = static_cast<unsigned char>(i);
        int x = c;
        ss << std::hex;
        ss << std::setw(2) << std::setfill('0') << x;
    }

    return ss.str();
}

QByteArray AppCore::strHex(const std::string &data)
{
    QByteArray tmp;
    for (size_t i = 0; i < data.size(); i += 2) {
        std::stringstream ss;
        ss << std::hex;
        std::string hexConv = std::string("") + data.at(i) + std::string("") + data.at(i + 1);
        ss << hexConv;
        int x = 0;
        ss >> x;
        char c = static_cast<char>(x);
        tmp.append(c);
    }
    return tmp;
}

void AppCore::encryptionBinFile(const QUrl &pathFile, const QString &key, const QString &dateTime, const qint16 &mainNumber, const qint16 &subNumber)
{
    QByteArray bin;
    QByteArray simpleKey;

    simpleKey = key.toLocal8Bit();

    //Read a bin file
    QFile file(pathFile.toLocalFile());
    file.open(QIODevice::ReadOnly);
    bin = file.readAll();
    file.close();

    //AES128 - ECB
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    //encode the bin file
    QByteArray encodedText = encryption.encode(bin, simpleKey);
    //decode the bin file
//    QByteArray decodedText = encryption.decode(encodedText, simpleKey);
    //get MD5 hash (no encoded file)
    QByteArray hashKey = QCryptographicHash::hash(encodedText, QCryptographicHash::Md5);

    qint32 sizeFirmware = encodedText.size();
    qint32 countFirmware = encodedText.count();
    qDebug() << countFirmware;

    QByteArray byteArray;
//    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    byteArray.append(static_cast<char>(mainNumber)); //1
    byteArray.append(static_cast<char>(subNumber)); //1

//    //New firmware version 2B
//    stream << mainNumber;
//    //Update sequence number 2B
//    stream << subNumber;

    //to class
//    versionFirmware =  QString(mainNumber);
//    subVersionFirmware =  QString(subNumber);

    QByteArray byteArraySize;
    QDataStream streamSize(&byteArraySize, QIODevice::WriteOnly);

    //Size of new firmware4B
    streamSize << sizeFirmware; //4
//    byteArraySize.append(sizeFirmware);
//    byteArraySize.at(subNumber);

    //MD5 16B
    const QByteArray& byteArrayMd5 = hashKey; //16 //22

    //Time and date of firmware creation 30B
    QByteArray byteArrayTimeDate;
//    QDataStream streamTimeDate(&byteArrayTimeDate, QIODevice::WriteOnly);
    byteArrayTimeDate = dateTime.toLocal8Bit();

    int fillNull =  CONST_TIME_HEAD_FIRMWARE - byteArrayTimeDate.size();
    //Add 0
    for (int i = 0; i < fillNull; i++) {
        byteArrayTimeDate.append(static_cast<char>(0));
    }
    //52

    //Reserve area (possibly adding new fields in the future) 202B
    QByteArray byteArrayReserve;
    byteArrayReserve.fill(0, CONST_RESERV_AREA);

    QByteArray byteArraySizeBare;
    QDataStream streamSizeBare(&byteArraySizeBare, QIODevice::WriteOnly);

    qint32 sizeFirmwareBare = bin.size();
    //Size of new firmware4B
    streamSizeBare << sizeFirmwareBare; //4

    //MD5 16B
    QByteArray hashKeyBare = QCryptographicHash::hash(bin, QCryptographicHash::Md5);
    const QByteArray& byteArrayMd5Bare = hashKeyBare; //16 //22


    //Final bin
    QByteArray byteArrayFinalBin;
//    QDataStream streamFinalBin(&byteArrayFinalBin, QIODevice::WriteOnly);
    byteArrayFinalBin =  byteArray + byteArraySize + byteArrayMd5 + byteArrayTimeDate + byteArraySizeBare + byteArrayMd5Bare + byteArrayReserve + encodedText;
    firmwareForDownload = byteArrayFinalBin;

// Only for testing strHex and strHex

//    std::string tmp = hexStr(byteArrayFinalBin);


//    QString str = QString::fromStdString(tmp.c_str());
//    QByteArray cmp = strHex(tmp);

//    if(byteArrayFinalBin == cmp){
//        qCritical() << "Ok";
//    }
//    else{
//        qCritical() << "Error";
//    }

    //qCritical() << str;
    //qCritical() << str.size();
    //Write a bin file
    QFile fileEnc(pathFile.toLocalFile() + ".enc");
    fileEnc.open(QIODevice::WriteOnly);
    fileEnc.write(byteArrayFinalBin);
    fileEnc.close();

    emit sendEncFileName("file://" + pathFile.toLocalFile() + ".enc");
    emit sendMD5FileName(QString(byteArrayMd5.toHex()));
    emit sendSizeFileName(QString::number(sizeFirmware));
}

void AppCore::getListOfStationsFirmware()
{
    for (const auto &listOfStation : listOfStations) {
        emit listOfStationsAndGroups(listOfStation.nameOfID, listOfStation.nameOfStation);
    }
}
