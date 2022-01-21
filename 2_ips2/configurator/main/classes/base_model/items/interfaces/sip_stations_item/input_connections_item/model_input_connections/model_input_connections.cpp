#include "classes/json.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "include/convert/string.h"
#include "include/tr.h"
#include "classes/base_types/function_types.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/model_input_connections/input_connection.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/model_input_connections/model_input_connections.h"

ModelInputConnections::ModelInputConnections(QObject* parent)
    : inherited(parent)
{
}
ModelInputConnections::connection_name* ModelInputConnections::defValue(uint number, FunctionType const& function, QString const& destination, uint priority) noexcept
{
    return new InputConnection(number, function, destination, priority);
}
int ModelInputConnections::rowCount(const QModelIndex&) const
{
    return inherited::items().size();
}
int ModelInputConnections::columnCount(const QModelIndex&) const
{
    return COUNT_COLUMNS_INPUT;
}
Qt::ItemFlags ModelInputConnections::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;

    return index.column() == 2
        ? Qt::ItemIsSelectable | Qt::ItemIsEnabled
        : Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
bool ModelInputConnections::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i)
        inherited::items().insert(position + rows - 1, *defValue(static_cast<uint>(position + 1)));
    inherited::endInsertRows();
    return true;
}
bool ModelInputConnections::removeRows(int position, int rows, const QModelIndex& index) {
    inherited::beginRemoveRows(index, position, position + rows - 1);
    for(auto i = 0; i != rows; ++i) {        
        inherited::items().removeAt(position);
    }
    inherited::endRemoveRows();
    return true;
}
QVariant ModelInputConnections::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const auto& item = inherited::item(index.row());
        if (index.column() == 0)
            return item.number();
        else if (index.column() == 1)
            return toString(item.function());
        else if (index.column() == 2)
            return item.destination();
        else if (index.column() == 3)
            return item.priority();
    }
    return {};
}
QVariant ModelInputConnections::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iInputConnections::tr("Input number");
            case 1:
                return iInputConnections::tr("Function");
            case 2:
                return iInputConnections::tr("Destination");
            case 3:
            return iInputConnections::tr("Priority");
        }
    }
    return QVariant();
}
bool ModelInputConnections::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    auto& item = inherited::item(index.row());
    if (role == CommentRole)
        item.setComment(value.toString());

    if (role == Qt::EditRole) {
        auto& item = inherited::item(index.row());

        if (index.column() == 0)
            item.setNumber(value.toUInt());
        else if (index.column() == 1) {            
            FunctionType type;
            function_type::fromString(value.toString(), type);
            if (!(type == FunctionType::Invalid || type == FunctionType::Unknown))
                item.setFunction(type);                        
        }
        else if (index.column() == 2)
                item.setDestination(value.toString());
        else if (index.column() == 3)
               item.setPriority(value.toUInt());

        emit(dataChanged(index, index, {role}));
    }
    return true;
}
QJsonArray ModelInputConnections::serialize() const {
    QJsonArray res;
    std::transform(items().cbegin(), items().cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
            {IPS_STR("number"), static_cast<int>(item.number())}
           ,{IPS_STR("function"), static_cast<int>(item.function())}
           ,{IPS_STR("destination"), item.destination()}
           ,{IPS_STR("priority"), static_cast<int>(item.priority())}
    });
    });
    return res;
}
bool ModelInputConnections::deserialize(QJsonArray const& arr)
{
    auto i = 0;
    auto pos = 0;
    for (auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = items().value(i);
        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("number"), number)
        item.setNumber(number);

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("function"), function)
        item.setFunction(static_cast<FunctionType>(function));

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("destination"), destination)
        item.setDestination(destination);

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("priority"), priority)
        item.setPriority(priority);
        insertItem(pos++, item);
   }
    return true;
}
