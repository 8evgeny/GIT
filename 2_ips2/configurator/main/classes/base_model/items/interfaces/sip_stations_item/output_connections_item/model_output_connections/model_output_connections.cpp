#include "classes/json.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "classes/oid.h"
#include "include/convert/string.h"
#include "include/tr.h"
#include <QCryptographicHash>
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/model_output_connections/output_connection.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/model_output_connections/model_output_connections.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/oid_helper.h"

ModelOutputConnections::ModelOutputConnections(QObject* parent)
    : inherited(parent)
{
}
ModelOutputConnections::connection_name* ModelOutputConnections::defValue(uint oid, QString const& name, uint number) noexcept
{
    auto result = new OutputConnection();
    result->setOid(oid);
    result->setName(name);
    result->setNumber(number);
    return result;
}
int ModelOutputConnections::rowCount(const QModelIndex&) const
{
    return inherited::items().size();
}
int ModelOutputConnections::columnCount(const QModelIndex&) const
{
    return COUNT_COLUMNS_CONNECTIONS;
}
Qt::ItemFlags ModelOutputConnections::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return  QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool ModelOutputConnections::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i)
        inherited::items().insert(position + rows - 1, *defValue(OidHelper::next(), IPS_STR("DefaultName"), static_cast<uint>(position+1)));

    inherited::endInsertRows();
    return true;
}
QVariant ModelOutputConnections::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto const& item = inherited::item(index.row());
        if (index.column() == 0)
            return item.oid();
        else if (index.column() == 1)
            return convert::truncate(item.name(), 64);
        else if (index.column() == 2)
             return item.number();
    }
    return {};
}
QVariant ModelOutputConnections::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iOtputConnections::tr("OID");
            case 1:
                return iOtputConnections::tr("Name");
            case 2:
                return iOtputConnections::tr("Number");
        }
     }
    return QVariant();
}
bool ModelOutputConnections::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;    

    auto& item = inherited::item(index.row());
    if (role == CommentRole)
        item.setComment(value.toString());    

    if (role == Qt::EditRole) {
        if (index.column() == 0) {
            QRegularExpression reg;
            reg.setPattern(IPS_STR("^[0]"));
            if (isOid(value) && !OidHelper::contains(toOid(value)) && !reg.match(value.toString()).hasMatch()) {
                item.setOid(toOid(value));
            }
        }
        if (index.column() == 1) {
            item.setName(value.toString());
        }
        if (index.column() == 2) {
            item.setNumber(static_cast<uint>(value.toInt()));
        }
        emit(dataChanged(index, index, {role}));
    }
    return true;
}
QJsonArray ModelOutputConnections::serialize() const
{
    QJsonArray res;
    std::transform(items().cbegin(), items().cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
            {IPS_STR("OID"), static_cast<int>(item.oid())}
          , {IPS_STR("name"), item.name()}
          , {IPS_STR("number"), static_cast<int>(item.number())}
    });
    });
    return res;
}
bool ModelOutputConnections::deserialize(QJsonArray const& arr)
{
    auto i = 0;
    auto pos = 0;
    for (auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = items().value(i);
        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("OID"), oid)
        item.setOid(oid);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("name"), name)
        item.setName(name);

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("number"), number)
        item.setNumber(number);
        insertItem(pos++, item);
   }
    return true;
}
