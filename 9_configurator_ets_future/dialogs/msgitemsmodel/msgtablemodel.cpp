#include <QString>
#include "msgtablemodel.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

MsgStringItem::MsgStringItem()
{
    this->datetime = QString::fromLocal8Bit("");
    this->cid = QString::fromLocal8Bit("");
    this->ip = QString::fromLocal8Bit("");
	this->msg = QString::fromLocal8Bit("");
}

MsgTableModel::MsgTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
	header_data << QObject::tr("DateTime") << QObject::tr("CID") << QObject::tr("IP") << QObject::tr("Message");
}

QVariant MsgTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
 
    if (index.row() >= list.size())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if (index.column() == 0 )
            return list.at(index.row())->datetime;
        if (index.column() == 1 )
            return list.at(index.row())->cid;
        if (index.column() == 2 )
            return list.at(index.row())->ip;
        if (index.column() == 3 )
            return list.at(index.row())->msg;
    }
    return QVariant();
}

MsgStringItem MsgTableModel::data(int row){
	return *list[row];
}

bool MsgTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if(index.column()==0){
            list.at(index.row())->datetime = value.toString();
        }
        if(index.column()==1){
            list.at(index.row())->cid = value.toString();
        }
        if(index.column()==2){
            list.at(index.row())->ip = value.toString();
        }
        if(index.column()==3){
            list.at(index.row())->msg = value.toString();
        }
        return true;
    }
    return false;
}

int MsgTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return list.size(); 
}
 
int MsgTableModel::columnCount(const QModelIndex &/*parent*/)    const
{
    return 4; 
}
 
void MsgTableModel::MyReset(){
	this->beginResetModel();
	this->endResetModel();
}

void MsgTableModel::clear(){
	list.clear();
	this->beginResetModel();
	this->endResetModel();
}

QVariant MsgTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(role != Qt::DisplayRole)
           return QVariant();
   if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
       return header_data.at(section); 
   }else{
	   return QString(QObject::tr("%1")).arg( section + 1 );
   }
}

Qt::ItemFlags MsgTableModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
        return Qt::ItemIsEnabled;
 
   return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void MsgTableModel::AppendRow(MsgStringItem* _item){
	list.append(_item);
} 

void MsgTableModel::save(QString filename){
	QJsonArray listMsg;
	MsgStringItem* pMsg;
	QJsonObject lMsg;
	QFile* _fileMsg;
	if (list.count() > 0){
		for(int _i = 0; _i < list.count(); _i++){
			pMsg = list.at(_i);
			QJsonObject obj;
			obj[QString::fromLocal8Bit("DT")] = pMsg->datetime;
			obj[QString::fromLocal8Bit("cid")] = pMsg->cid;
			obj[QString::fromLocal8Bit("ip")] = pMsg->ip;
			obj[QString::fromLocal8Bit("MSG")] = pMsg->msg;
			listMsg.append(obj);
		}
		lMsg[QString::fromLocal8Bit("Messages")] = listMsg;
		_fileMsg = new QFile(filename);
		if (!_fileMsg->open(QIODevice::WriteOnly | QIODevice::Text)){
			return;
		}
		_fileMsg->write(QString::fromLocal8Bit(QJsonDocument(lMsg).toJson()).toLocal8Bit());
		_fileMsg->close();
		delete _fileMsg;
	}
}

void MsgTableModel::load(QString filename){
	QJsonArray listMsg;
	MsgStringItem* pMsg;
	QFile* _fileMsg;
	QByteArray ba;
	_fileMsg = new QFile(filename);
	if (!_fileMsg->open(QIODevice::ReadOnly | QIODevice::Text)){
		return;
	}
	ba = _fileMsg->readAll();
	_fileMsg->close();
	delete _fileMsg;
	list.clear();
	QJsonDocument doc = QJsonDocument::fromJson(ba);
	QJsonObject mapSrcObj = doc.object();
	listMsg = mapSrcObj[QString::fromLocal8Bit("Messages")].toArray();
	foreach (const QJsonValue & value, listMsg) {
		QJsonObject obj = value.toObject();
		pMsg = new MsgStringItem();
		pMsg->datetime = obj[QString::fromLocal8Bit("DT")].toString();
		pMsg->cid = obj[QString::fromLocal8Bit("cid")].toString();
		pMsg->ip = obj[QString::fromLocal8Bit("ip")].toString();
		pMsg->msg = obj[QString::fromLocal8Bit("MSG")].toString();
		list.append(pMsg);
	}
	MyReset();
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
