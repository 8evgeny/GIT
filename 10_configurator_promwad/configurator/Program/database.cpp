#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    this->connectToDataBase();
}

DataBase::~DataBase()
{

}


void DataBase::testInsert()
{
    int random = rand();
    for (int i = 0; i < 1; i++) {
        QVariantList data;
        data.append(i);
        data.append("Name" + QString::number(i + 1));
        data.append("Type" + QString::number(i + 2));
        data.append(random + 3);
        data.append(random + 4);
        data.append("192.168.2." + QString::number(i + 100));
        data.append("31.32.33.01.12.34");
        inserIntoTable(data);
    }
}


void DataBase::connectToDataBase()
{
    this->openDataBase();
}


bool DataBase::restoreDataBase()
{
    if (this->openDataBase()) {
        if (!this->createTable()) {
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug() << db.drivers();
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    db.setUserName("root");
    db.setPassword("1234567890");
    if (db.open()) {
        return true;
    } else {
        qDebug() << db.lastError().text();
        db.setDatabaseName("");
        if (!db.open()) {
            return false;
        }
        QSqlQuery query(db);
        query.prepare(QString("CREATE DATABASE IF NOT EXISTS ")
                      + QString(DATABASE_NAME));
        query.exec();
        db.close();
        if (!restoreDataBase())
            return false;
        else
            return true;
    }
}


void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createTable()
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE " TABLE " ("
                    "id INTEGER PRIMARY KEY AUTO_INCREMENT, "
                    TABLE_NUMBER        " INTEGER           NOT NULL,"
                    TABLE_NAME          " VARCHAR(255)      NOT NULL,"
                    TABLE_TYPE          " VARCHAR(255)      NOT NULL,"
                    TABLE_BUTTONS       " INTEGER           NOT NULL,"
                    TABLE_GROUP         " INTEGER           NOT NULL,"
                    TABLE_IP            " VARCHAR(255)      NOT NULL,"
                    TABLE_MAC           " VARCHAR(255)      NOT NULL"
                    " )"
                   )) {
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::inserIntoTable(const QVariantList &data)
{

    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_NUMBER ", "
                  TABLE_NAME ", "
                  TABLE_TYPE ", "
                  TABLE_BUTTONS ", "
                  TABLE_GROUP ", "
                  TABLE_IP ", "
                  TABLE_MAC " ) "
                  "VALUES (:Number, :Name, :Type, :Buttons, :GroupNumber, :Ip, :Mac )");
    query.bindValue(":Number",      data[0].toInt());
    query.bindValue(":Name",        data[1].toString());
    query.bindValue(":Type",        data[2].toString());
    query.bindValue(":Buttons",     data[3].toInt());
    query.bindValue(":GroupNumber", data[4].toInt());
    query.bindValue(":Ip",          data[5].toString());
    query.bindValue(":Mac",         data[6].toString());

    if (!query.exec()) {
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
