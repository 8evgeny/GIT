#include "model.h"

Model::Model(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QVariant Model::data(const QModelIndex &index, int role) const
{


    int columnId = role - Qt::UserRole - 1;

    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> Model::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[NumberRole] = "number";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[ButtonsRole] = "buttons";
    roles[GroupNumberRole] = "groupNumber";
    roles[IpRole] = "ip";
    roles[MacRole] = "mac";
    return roles;
}
