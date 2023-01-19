#include "appcore.h"
#include <utility>
#include <QDir>
#include <QFile>
#include <QModelIndex>
#include <QSqlRecord>
#include <QThread>
#include <QTimer>
#include <qdatastream.h>
#include "receiver.h"

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    QStringList stringList;
    QFile textFile("history");
    if (textFile.open(QIODevice::ReadOnly)) {
        //(open the file for reading, etc.)
        QTextStream textStream(&textFile);
        while (true) {
            QString line = textStream.readLine();
            if (line.isNull()) {
                break;
            }
            stringList.append(line);
            listOfFiles.append(line);
        }
        textFile.close();
    } else {
        //empty list of files
        listOfFiles.append("");
        listOfFiles.append("");
        listOfFiles.append("");
    }

}

void AppCore::setCurrentLanguage(int lang)
{
    if (lang == RuLang::ENG)
        currentLanguage = 0;

    if (lang == RuLang::RUS)
        currentLanguage = 1;
}

void AppCore::receiveFromQmlSubscriber(QString str, qint32 number)
{
    emit sendToQmlSubscriber(std::move(str), number);
}

void AppCore::receiveFromQmlMacro(const QString &nameOfMacro, qint32 nameOfPriority, const QString &nameOfMsg, qint32 nameOfRepeat)
{
    ListOfMacros a;

    for (const auto &listOfMacro : listOfMacros) {
        if (listOfMacro.nameOfMacro == nameOfMacro) {
            emit sendErrorMacroExists();
            return;
        }
    }

    a.nameOfMacro =  nameOfMacro;
    a.nameOfPriority =  nameOfPriority;
    a.nameOfMsg =  nameOfMsg;
    a.nameOfRepeat =  nameOfRepeat;

    listOfMacros.append(a);
}

void AppCore::changesOfMacro(const QString &nameOfMacro, qint32 nameOfPriority, const QString &nameOfMsg, qint32 nameOfRepeat)
{
    for (auto &listOfMacro : listOfMacros) {
        if (listOfMacro.nameOfMacro == nameOfMacro) {
            listOfMacro.nameOfPriority = nameOfPriority;
            listOfMacro.nameOfMsg = nameOfMsg;
            listOfMacro.nameOfRepeat = nameOfRepeat;

            return;
        }
    }

    emit sendErrorMacroExistsChanges();
}

void AppCore::getListOfMacros()
{
    for (const auto &listOfMacro : listOfMacros) {
        emit sendListNameOfMacro(listOfMacro.nameOfMacro);
    }
}

void AppCore::removeMacro(qint32 number)
{
    listOfMacros.removeAt(number);
}

void AppCore::fillMacro(const QString &name)
{
    for (int i = 0; i < listOfMacros.size(); i++) {
        if (listOfMacros.at(i).nameOfMacro == name) {
            emit sendSelectedMacro(i, listOfMacros[i].nameOfMacro, listOfMacros[i].nameOfPriority, listOfMacros[i].nameOfMsg, listOfMacros[i].nameOfRepeat);
            return;
        }
    }
}

void AppCore::saveStation(const QString &nameOfStation, const QString &idOfStation, const QString &ipOfStation)
{
    ListOfStations st;

    st.nameOfStation = nameOfStation;
    st.nameOfID = idOfStation;
    st.ip  = ipOfStation;
    listOfStations.append(st);

    //В зависимости от имени добавляем поля
    if (nameOfStation.contains("SL1"))
    {
        for (int i = 0; i < 8 ;++i)
        {
            addNewKeyBlock(idOfStation);
            listOfStations.last().listOfKeys[i].numberOfKey = QString::number(i+1);
            listOfStations.last().listOfKeys[i].nameOfKey =  "input " + QString::number(i+1);
//            listOfStations.last().listOfKeys[i].functionOfKey = "6"; //Direct connection simplex
            listOfStations.last().listOfKeys[i].modeKey = "2"; //No Fixation
            listOfStations.last().listOfKeys[i].prioriryKey = "1";
        }
    }
    else if(nameOfStation.contains("PDO16") && !nameOfStation.contains("N"))
    {
        for (int i = 0; i < 16 ;++i)
        {
            addNewKeyBlock(idOfStation);
            listOfStations.last().listOfKeys[i].numberOfKey = QString::number(i+1);
            listOfStations.last().listOfKeys[i].nameOfKey =  "key " + QString::number(i+1);
            listOfStations.last().listOfKeys[i].modeKey = "2"; //No Fixation
            listOfStations.last().listOfKeys[i].prioriryKey = "1";
        }
    }
    else if(nameOfStation.contains("PDO32"))
    {
        for (int i = 0; i < 32 ;++i)
        {
            addNewKeyBlock(idOfStation);
            listOfStations.last().listOfKeys[i].numberOfKey = QString::number(i+1);
            listOfStations.last().listOfKeys[i].nameOfKey =  "key " + QString::number(i+1);
            listOfStations.last().listOfKeys[i].modeKey = "2"; //No Fixation
            listOfStations.last().listOfKeys[i].prioriryKey = "1";
        }
    }
    else if(nameOfStation.contains("PDO16N"))
    {
        for (int i = 0, j=0; i < 26 ; ++i, ++j)
        {
                addNewKeyBlock(idOfStation);
                listOfStations.last().listOfKeys[i].numberOfKey = QString::number(j+1);
                listOfStations.last().listOfKeys[i].nameOfKey =  "key " + QString::number(j+1);
                if (i > 15)
                {
                    listOfStations.last().listOfKeys[i].nameOfKey =  "telephone key " + QString::number(j+1-50);
                    listOfStations.last().listOfKeys[i].functionOfKey = "5"; //Telephone
                }
                else
                {
                    listOfStations.last().listOfKeys[i].nameOfKey =  "key " + QString::number(j+1);
//                    listOfStations.last().listOfKeys[i].functionOfKey = "6";
                }

                listOfStations.last().listOfKeys[i].modeKey = "2"; //No Fixation
                listOfStations.last().listOfKeys[i].prioriryKey = "1";
                if (i == 15)
                    j = 49;
        }
    }
    listOfStations.last().gateway = "0.0.0.0";
    listOfStations.last().maskNetwork = "255.255.255.0";

}

void AppCore::deleteStation(const QString &nameOfStation)
{
    for (int i = 0; i < listOfStations.size(); i++) {
        if (listOfStations.at(i).nameOfID == nameOfStation) {
            listOfStations.removeAt(i);
            return;
        }
    }
}

void AppCore::getListOfStations()
{
    for (const auto &listOfStation : listOfStations) {
        emit sendListOfStations(listOfStation.nameOfStation, listOfStation.nameOfID);
    }
}

void AppCore::createGroup(const QString &nameOfGroup, const QString &idOfGroup)
{
    ListOfGroups lst;

    lst.nameOfGorup = nameOfGroup;
    lst.nameOfID = idOfGroup;

    listOfGroups.append(lst);

}

void AppCore::deleteGroup(const QString &nameOfGroup)
{
    for (int i = 0; i < listOfGroups.size(); i++) {
        if (listOfGroups.at(i).nameOfGorup == nameOfGroup) {
            listOfGroups.removeAt(i);
            return;
        }
    }
}

void AppCore::resetCurrentStateForProject()
{
    listOfGroupsSaved.clear();
    listOfGroupsSaved.clear();
    listOfMacros.clear();
    listOfStations.clear();
    listOfGroups.clear();
    firmwareForDownload.clear();

    emit clearListOfStations();
    emit clearListOfGroups();
    emit clearVisibleSubsriber();

    //set flag that it is a new project
    stateNewProejct = true;
    //update counters
    emit setCounterDigitalStation(99); //100 - 1
    emit setCounterGroup(-1); //0 - 1
}

void AppCore::needCloseMainWindow()
{
    emit closeMainWindow();
}

void AppCore::checkChanges(bool state)
{
    if ((listOfGroupsSaved.size() != listOfGroups.size()) || (listOfStationsSaved.size() != listOfStations.size())) {
        //emit menu exist changes
        if (state) emit windowChangesBeforeSave();
        else emit windowChangesMenuSave();
        return;
    }

    if ((listOfGroupsSaved.size() == listOfGroups.size()) || (listOfStationsSaved.size() == listOfStations.size())) {
        for (int i = 0; i < listOfGroupsSaved.size(); i++) {
            if (listOfGroupsSaved.at(i).nameOfID != listOfGroups.at(i).nameOfID) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }

            if (listOfGroupsSaved.at(i).nameOfGorup != listOfGroups.at(i).nameOfGorup) {
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                //emit menu exist changes
                return;
            }

            if (listOfGroups.at(i).stations.size() == listOfGroupsSaved.at(i).stations.size()) {
                for (int j = 0; j < listOfGroups.at(i).stations.size(); j++) {
                    if (listOfGroupsSaved.at(i).stations.at(j).nameOfStation != listOfGroups.at(i).stations.at(j).nameOfStation) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if (listOfGroupsSaved.at(i).stations.at(j).nameOfID != listOfGroups.at(i).stations.at(j).nameOfID) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                }
            } else {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }
        }

        for (int i = 0; i < listOfStations.size(); i++) {
            if (listOfStationsSaved.at(i).nameOfID != listOfStations.at(i).nameOfID) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }
            if (listOfStationsSaved.at(i).nameOfStation != listOfStations.at(i).nameOfStation) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }

            if (listOfStationsSaved.at(i).ip != listOfStations.at(i).ip) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }

            if (listOfStationsSaved.at(i).gateway != listOfStations.at(i).gateway) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }

            if (listOfStationsSaved.at(i).maskNetwork != listOfStations.at(i).maskNetwork) {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }

            if (listOfStations.at(i).listOfKeys.size() == listOfStationsSaved.at(i).listOfKeys.size()) {
                for (int j = 0; j < listOfStations.at(i).listOfKeys.size(); j++) {
                    if ((listOfStations.at(i).listOfKeys.at(j).modeKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).modeKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).uidOfKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).uidOfKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).nameOfKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).nameOfKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).assignedKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).assignedKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).numberOfKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).numberOfKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).prioriryKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).prioriryKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                    if ((listOfStations.at(i).listOfKeys.at(j).functionOfKey) != (listOfStationsSaved.at(i).listOfKeys.at(j).functionOfKey)) {
                        //emit menu exist changes
                        if (state) emit windowChangesBeforeSave();
                        else emit windowChangesMenuSave();
                        return;
                    }
                }
            } else {
                //emit menu exist changes
                if (state) emit windowChangesBeforeSave();
                else emit windowChangesMenuSave();
                return;
            }
        }
    }
    if (state) emit closeMainWindow();
    return;
}

void AppCore::getStatusMsg(const QString &id, const QString &msg)
{
    emit createWindowStatus(id, msg);
}

void AppCore::getCurrentIndexInList()
{
    if (listOfStations.size() > 0) {
        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfStations.back().nameOfID.split(rx, QString::SkipEmptyParts);

        emit setCounterDigitalStation(list.at(1).toInt());
    }

    if (listOfGroups.size() > 0) {
        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroups.back().nameOfID.split(rx, QString::SkipEmptyParts);
        emit setCounterGroup(list.at(1).toInt());
    }
}

void AppCore::exitActionFromMenu(){
    emit exitFromMenu();
}

void AppCore::deleteKeyPadFromStation(const QString& nameId)
{
    qDebug() << "deleteKeyPadFromStation";
    int i = 0;
    for (; i<listOfStations.size(); ++i )
    {
        if(listOfStations.at(i).nameOfID == nameId)
            break;
    }
    //i - индекс нужной станции
    for (int k = 0; k < listOfStations.at(i).listOfKeys.size(); ++k )
    {
        if (listOfStations.at(i).listOfKeys.at(k).numberOfKey.toInt() >=50)
        {
            //удаляем найденную клавишу
//            qDebug() << "delete Key" <<listOfStations.at(i).listOfKeys.at(k).numberOfKey;
            listOfStations[i].listOfKeys.removeAt(k);
            //Нужно, иначе удалится только половина
            --k;
        }
    }
    //Обновление вида
    sendCurrentIndexOfDigitalStation(0,nameId);
    QThread::msleep(100);
}

void AppCore::addKeyPadToStation(const QString& nameId)
{
    qDebug() << "addKeyPadToStation";
    int m = 0;
    for (; m<listOfStations.size(); ++m )
    {
        if(listOfStations.at(m).nameOfID == nameId)
            break;
    }
    //m - индекс нужной станции
    for (int i = 51; i<= 60 ; ++i)
    {
            addNewKeyBlock(nameId);
            listOfStations[m].listOfKeys.last().numberOfKey = QString::number(i);
            if (i < 60)
                listOfStations[m].listOfKeys.last().nameOfKey = "telephone key " + QString::number(i - 50);
            if (i == 60)
                listOfStations[m].listOfKeys.last().nameOfKey = "telephone key " + QString::number(0);
            listOfStations[m].listOfKeys.last().functionOfKey = "5"; //Telephone
            listOfStations[m].listOfKeys.last().modeKey = "2"; //No Fixation
            listOfStations[m].listOfKeys.last().prioriryKey = "1";
    }
    //Обновление вида
    sendCurrentIndexOfDigitalStation(0,nameId);
}

//Проверка содержит ли станция телефонные клавиши
void AppCore::checkTelButtons(const QString& nameId)
{
    int i = 0;
    for (; i<listOfStations.size(); ++i )
    {
        if(listOfStations.at(i).nameOfID == nameId)
            break;
    }
    //i - индекс нужной станции
    for (int k = 0; k < listOfStations.at(i).listOfKeys.size(); ++k )
    {
        if (listOfStations.at(i).listOfKeys.at(k).numberOfKey.toInt() >=50)
        {
            qDebug() << "CheckBox ON";
            emit setCheckBox();
            return;
        }
    }
    qDebug() << "CheckBox OFF";
    emit unsetCheckBox();
}
