#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define DATABASE_HOSTNAME   "localhost"
#define DATABASE_NAME       "my_database"

#define TABLE                   "TableProgram"
#define TABLE_NUMBER            "number"
#define TABLE_NAME              "name"
#define TABLE_TYPE              "type"
#define TABLE_BUTTONS           "buttons"
#define TABLE_GROUP             "groupNumber"
#define TABLE_IP                "ip"
#define TABLE_MAC               "mac"

class DataBase : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @fn DataBase
     * @param parent
     */
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    bool inserIntoTable(const QVariantList &data);
    void testInsert();

private:
    QSqlDatabase    db;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();
};

#endif
