/*!
 * @file msgtablemodel.h
 * @file msgtablemodel.cpp
 * @brief This file includes the class of the message list data display model 
 */

/*! 
\defgroup msgtablemodel_module the class of the message list data display model
*/
#pragma once
#include "include/typedefs.h"
#include <QtCore\QAbstractTableModel>
#include <QtCore\QList>
#include <QtCore\QFile>
#include <QtCore\QStringList>
#include <QtCore\QJsonDocument>
#include <QtCore\QJsonArray>
#include <QtCore\QJsonObject>
#include "string.h"
#include "msgtablemodel_pch.h"
#include "msgtablemodel_global.h"

 /*!
 \defgroup MsgStringItem_module
  * \class MsgStringItem
  * \brief Class contains description single string of the message
  \ingroup msgtablemodel_module
  */
class MSGTABLEMODEL_SHARED_EXPORT MsgStringItem
{
public:
/*!
Function creation class
*/
    MsgStringItem();
///Date and time create message
    QString datetime;
///CID identificator source message device
	QString cid;		
///IP address source message device
	QString ip;	
///message
    QString msg;
};
/*!
 \defgroup MsgTableModel_module
  * \class MsgTableModel
  * \brief Class contains list of strings of the message
  \ingroup msgtablemodel_module
  */
class MSGTABLEMODEL_SHARED_EXPORT MsgTableModel final: public QAbstractTableModel {
    Q_OBJECT
    IPS_DISABLE_COPY(MsgTableModel)

 public:
    using inherited = QAbstractTableModel;
    using class_name = MsgTableModel;

 public:
/*!
 \fn MsgTableModel(QObject *parent = 0)
 \brief Function creation class
 \param [in] parent pointer to the parent class
*/
	explicit MsgTableModel(QObject *parent = 0);

public:
/*!
* \fn void AppendRow(MsgStringItem* _item);
* \brief function append single row to list messages
* \param _item [in] Pointer into @ref MsgStringItem single message string 
*/
	void AppendRow(MsgStringItem* _item);
/*!
* \fn QVariant data(const QModelIndex &index, int role) const;
* \brief function gets selected single data from list messages
* \param index [in] index of model data
* \param role [in] mode model data
* \return QVariant
*/
	QVariant data(const QModelIndex &index, int role) const;
/*!
* \fn MsgStringItem data(int row);
* \brief function gets selected single data from list messages
* \param row [in] number row into list messages
* \return @ref MsgStringItem
*/
	MsgStringItem data(int row);
/*!
* \fn bool setData(const QModelIndex &index, const QVariant &value, int role);
* \brief function insert single message to list
* \param index [in] index model data
* \param role [in] mode model data
* \param value [in] message
* \return bool insert result
*/
    bool setData(const QModelIndex &index, const QVariant &value, int role);
/*!
* \fn int rowCount(const QModelIndex &parent=QModelIndex()) const;
* \brief function for returning the number of rows in a list
* \param parent [in] index model data
* \return int number of rows in a list
*/
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
/*!
* \fn int columnCount(const QModelIndex &parent) const;
* \brief function for returning the number of columns in a list
* \param [in] parent index model data
* \return int number of columns in a list
*/
    int columnCount(const QModelIndex &parent) const;
/*!
* \fn void MyReset();
* \brief data model update function
* \return void
*/
	void MyReset();
/*!
* \fn QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
* \brief function for returning contents header cells
* \param section [in] number section
* \param orientation [in] mode orientation header
* \param role [in] mode display header
* \return QVariant
*/
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
/*!
* \fn Qt::ItemFlags flags(const QModelIndex &index) const;
* \brief function for returning state model data
* \param index [in] index model data
* \return Qt::ItemFlags
*/
    Qt::ItemFlags flags(const QModelIndex &index) const;
/*!
* \fn void clear();
* \brief function for clear contents in a list
* \return void
*/
	void clear();
/*!
* \fn void save(QString filename);
* \brief function for save contents list messages in file
* \param [in] filename filename to save
* \return void
*/
	void save(QString filename);
/*!
* \fn void load(QString filename);
* \brief function for load contents list messages from file
* \param [in] filename filename 
* \return void
*/
	void load(QString filename);
 private:
///List of messages
    QList<MsgStringItem *> list;
///List of header content 
    QStringList header_data;
};
