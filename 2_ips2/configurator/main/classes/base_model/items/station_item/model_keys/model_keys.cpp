#include "model_keys.h"
#include "classes/json.h"
#include "classes/defines.h"
#include "classes/base_model/items/station_item/model_keys/key_helper.h"

ModelKeys::ModelKeys(QObject *parent)
    : inherited(parent)    
{
}
int ModelKeys::columnCount(const QModelIndex&) const
{
    return 5;
}
ModelKeys::keys_name* ModelKeys::defValue(uint id) const noexcept
{
    auto result = new keys_name();
    result->setId(id);
    return result;
}
bool ModelKeys::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i)
        inherited::items().insert(position + rows - 1, *defValue(KeyHelper::next(m_parent)));

    inherited::endInsertRows();
    return true;
}
QVariant ModelKeys::data(const QModelIndex &index, int role) const
{    
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const auto& key = inherited::item(index.row());
        if (index.column() == 0)
            return key.id();
        else if (index.column() == 1)
            return convert::truncate(key.name(), 64);
        else if (index.column() == 2)
            return toString(key.function());
        else if (index.column() == 3)
            return key.destination();
        else if (index.column() == 4 && key.priority() > 0) {
            return key.priority();
        }
    }
    return {};
}
QVariant ModelKeys::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iDesc::tr("Key");
            case 1:
                return iDesc::tr("Name");
            case 2:
                return iDesc::tr("Function");
            case 3:
                return iDesc::tr("Destination");
            case 4:
                return iDesc::tr("Priority");
        }
     }
    return {};
}
bool ModelKeys::setData(const QModelIndex &index, const QVariant &value, int role)
{    
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& key = inherited::item(index.row());
        auto const& val = value.toString();

        if (index.column() == 0) {
            if (isKey(value) && !KeyHelper::contains(value.toUInt(), m_parent) && value.toInt() !=0)
                key.setId(value.toUInt());
        }
        else if (index.column() == 1)
            key.setName(convert::truncate(val, 64));
        else if (index.column() == 2) {
            auto type = value.value<FunctionType>();
            if (!(type == FunctionType::Invalid || type == FunctionType::Unknown))
                key.setFunction(type);
        }
        else if (index.column() == 3) {
                QRegExp rx(IPS_STR(R"([Cc][Ii][Dd]\d+)"));
                rx.setMinimal(false);
                key.setDestination(convert::truncate(val, 256));
        }        
        else if (index.column() == 4) {
            if(convert::isUint(value.toString()) && convert::toUint(value) < 100)
                key.setPriority(convert::toUint(value));
        }
        emit(dataChanged(index, index, {role}));
    }
    return true;
}
Qt::ItemFlags ModelKeys::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return index.column() == 2 || index.column() == 3
        ? Qt::ItemIsSelectable | Qt::ItemIsEnabled
        : Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
QJsonArray ModelKeys::serialize() const
{
    QJsonArray res;
    std::transform(items().cbegin(), items().cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
            {IPS_STR("id"), static_cast<int>(item.id())}
          , {IPS_STR("name"), item.name()}
          , {IPS_STR("function"), static_cast<int>(item.function())}
          , {IPS_STR("destination"), item.destination()}
          , {IPS_STR("priority"), static_cast<int>(item.priority())}
          , {IPS_STR("pretone_id"), static_cast<int>(item.pretone().id())}
          , {IPS_STR("pretone_checked"), QString::number(static_cast<int>(item.pretoneChecked()))}
          , {IPS_STR("pretone_duration"), static_cast<int>(item.pretone().duration())}
          , {IPS_STR("pretone_times"), item.times()}
          , {IPS_STR("activate_lines"), item.activateControlLines()}
         });
    });
    return res;
}
bool ModelKeys::deserialize(QJsonArray const& arr)
{    
    auto i = 0;
    auto pos = 0;
    for (auto const& a: arr) {        
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = items().value(i);
        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("id"), id)
        item.setId(id);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("name"), name)
        item.setName(name);

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("function"), function)
        item.setFunction(static_cast<FunctionType>(function));

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("destination"), destination)
        item.setDestination(destination);

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("priority"), priority)
        item.setPriority(priority);

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("pretone_id"), pretone_id)
        item.setPretoneId(pretone_id);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("pretone_checked"), pretone_checked)
        item.setPretoneChecked(pretone_checked.toInt());

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("pretone_duration"), pretone_duration)
        item.setPretoneDuration(static_cast<DurationType>(pretone_duration));

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("pretone_times"), pretome_times)
        item.setTimes(pretome_times);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("activate_lines"), activateLines)
        item.setActivateControlLines(activateLines);       

        insertItem(pos++, item);
   }
   return true;
}
