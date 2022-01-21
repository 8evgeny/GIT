#include "include/log.h"
#include "digital_stations.h"
#include "include/convert/string.h"
#include "../include/validators/string_validator.h"
#include "classes/base_model/items/stations_item/stations_item.h"

ModelDigitalStations::ModelDigitalStations(QObject *parent)
    : inherited(parent)    
{   
}
int ModelDigitalStations::columnCount(const QModelIndex&) const
{
    return 2;
}
QVariant ModelDigitalStations::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return QVariant();

    const auto &contact = inherited::items()[index.row()];
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return contact.nameDigitalStation();
        else if (index.column() == 1) {
            if (contact.cid())
                return contact.cid();
            else
                return index.row()+1;
         }
    }
    if (role == CidRole)
        return contact.cid();
    return QVariant();
}
QVariant ModelDigitalStations::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iDesc::tr("Name");
            case 1:
                return iDesc::tr("CID");
        }
     }
    return QVariant();
}
bool ModelDigitalStations::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& contact = inherited::items()[index.row()];
        if (index.column() == 0)
            contact.setNameDigitalStation(convert::truncate(value.toString(), 256));
        if (index.column() == 1)
            contact.setId(value.toUInt());
    }
    return true;
}
QJsonArray ModelDigitalStations::serialize() const
{
    QJsonArray res;
    for (auto const& item: inherited::items()) {        
         res.push_back(QJsonObject {
              {IPS_STR("name"), item.nameDigitalStation()}
            , {IPS_STR("cid"), QString::number(static_cast<int>(item.cid()))}
        });
    }
    return res;
}
bool ModelDigitalStations::deserialize(QJsonArray const& arr)
{
    for(auto a: arr) {
       if (!a.isUndefined() && a.isObject()) {
          value_t val;
          val.setNameDigitalStation(a.toObject().value(IPS_STR("name")).toString());
          val.setCid(a.toObject().value(IPS_STR("cid")).toString().toUInt());
          inherited::items().append(val);
       }
       else
           return LOG_ERROR(false, "Stations is not an object");
    }
    return true;
}
