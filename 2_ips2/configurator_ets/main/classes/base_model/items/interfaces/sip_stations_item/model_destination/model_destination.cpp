#include <gt/json.h>
#include "classes/app.h"
#include "classes/defines.h"
#include "classes/json.h"
#include "include/convert/string.h"
#include "classes/base_model/items/interfaces/sip_stations_item/model_destination/model_destination.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_types/protocol_types.h"
#include "classes/base_types/registration_types.h"

ModelDestination::ModelDestination(QObject* parent)
    : inherited(parent)
{
}
int ModelDestination::rowCount(const QModelIndex&) const
{
    return inherited::items().size();
}
int ModelDestination::columnCount(const QModelIndex&) const
{
    return COUNT_COLUMNS_DESTINATION;
}
Qt::ItemFlags ModelDestination::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return  QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool ModelDestination::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i) {
         auto item = new Destination(IPS_STR("PABX-1"), IPS_STR("192.168.10.130"), ProtocolType::UDP, RegistrationType::ON);
         inherited::items().insert(position + rows - 1, *item);
    }    
    inherited::endInsertRows();
    return true;
}
QVariant ModelDestination::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto const& item = inherited::item(index.row());
        if (index.column() == 0)
            return convert::truncate(item.name(), 64);
        else if (index.column() == 1)
             return item.ip();
        else if (index.column() == 2) {
                return role == Qt::EditRole
                                ? QVariant::fromValue<ProtocolType>(item.protocol())
                                : toString(item.protocol());
        }
        else if (index.column() == 3)
            return toString(item.registr());
    }
    return {};
}
QVariant ModelDestination::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iDestination::tr("Name");
            case 1:
                return iDestination::tr("IP");
            case 2:
                return iDestination::tr("Protocol");
            case 3:
                return iDestination::tr("Registration");
        }
     }
    return QVariant();
}
bool ModelDestination::setData(const QModelIndex& index, const QVariant& value, int role)
{    
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& item = inherited::item(index.row());
        if (index.column() == 0)
            item.setName(value.toString());
        if (index.column() == 1)
            item.setIp(value.toString());
        if (index.column() == 2) {
            auto type = value.value<ProtocolType>();
            if (!(type == ProtocolType::Invalid || type == ProtocolType::Unknown))
                item.setProtocol(type);
        }
        if (index.column() == 3) {
            auto type = value.value<RegistrationType>();
            if (!(type == RegistrationType::Invalid || type == RegistrationType::Unknown))
                item.setRegistration(type);
        }
    }
    return true;
}
QJsonArray ModelDestination::serialize() const {
    QJsonArray res;   
    std::transform(items().cbegin(), items().cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
            {IPS_STR("name"), item.name()}
           ,{IPS_STR("ip"), item.ip()}
           ,{IPS_STR("protocol"), static_cast<int>(item.protocol())}
           ,{IPS_STR("registration"), static_cast<int>(item.registr())}
    });
    });
    return res;
}
bool ModelDestination::deserialize(QJsonArray const& arr)
{
    auto i = 0;
    auto pos = 0;
    for (auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = items().value(i);
        IPS_JSON_TO_STR(a.toObject(), IPS_STR("name"), name)
        item.setName(name);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("ip"), ip)
        item.setIp(ip);

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("protocol"), protocol)
        item.setProtocol(static_cast<ProtocolType>(protocol));

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("registration"), registration)
        item.setRegistration(static_cast<RegistrationType>(registration));
        insertItem(pos++, item);
   }
    return true;
}
