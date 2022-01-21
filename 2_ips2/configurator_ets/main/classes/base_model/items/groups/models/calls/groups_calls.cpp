#include "include/log.h"
#include "groups_calls.h"
#include "main/classes/cid.h"
#include "main/classes/gid.h"
#include "include/convert/string.h"
#include "../include/validators/string_validator.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/groups/groups_selector/gid_helper.h"

ModelGroups::ModelGroups(QObject* parent)
    : inherited(parent)
{
}
int ModelGroups::columnCount(const QModelIndex&) const
{
    return 2;
}
QVariant ModelGroups::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return QVariant();    

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const auto &contact = inherited::items()[index.row()];

        if (index.column() == 1) {
            return contact.nameGroupCall();
        }
        else if (index.column() == 0) {
                return (contact.gid())
                    ? contact.gid()
                    : static_cast<gid_t>(index.row() + 1);
             }
    }
    return QVariant();
}
QVariant ModelGroups::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iDesc::tr("GID");
            case 1:
                return iDesc::tr("Name");
        }
     }
    return QVariant();
}
ModelGroups::value_t* ModelGroups::defValue(gid_t gid) const noexcept
{
    auto result = new value_t();
    result->setGid(gid);
    return result;
}
bool ModelGroups::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i) {
        defValue(GidHelper::next());
        inherited::items().insert(position + rows - 1, *defValue(GidHelper::next()));
    }
    inherited::endInsertRows();
    return true;
}
bool ModelGroups::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    auto &contact = inherited::items()[index.row()];
    if (role == Qt::EditRole) {        
        if (index.column() == 0)
            contact.setNameGroupCall(convert::truncate(value.toString(), 256));
        if (index.column() == 1 && isGid(value) && !GidHelper::contains(toGid(value.toUInt())) && value !=0)
            contact.setGid(static_cast<gid_t>(value.toInt()));
    }
    return true;
}
bool ModelGroups::setListStation(const QModelIndex &index, const QList<ModelGroups::station_t*>& stations) noexcept
{    
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    auto &contact = inherited::items()[index.row()];
    contact.setStationList(stations);    
    return true;
}
QList<ModelGroups::station_t*>const& ModelGroups::stationList(const QModelIndex &index) const noexcept
{
    LOG_ERROR(index.isValid(), "index must be is valid");

    auto &contact = inherited::items()[index.row()];
    return contact.stationList();
}
QJsonArray ModelGroups::serializeListStations(GroupCall const& item) const noexcept
{
    QJsonArray res;
    auto stations = item.stationList();
    if (!stations.isEmpty()) {
        for (auto station : stations)
            res.push_back(QJsonObject {{IPS_STR("hashStation"), station->toHash()}});
    }
    return res;
}
QList<ModelGroups::station_t*> ModelGroups::deserializeListStations(QJsonObject obj) const noexcept
{
    QList<ModelGroups::station_t*> listStations;
    QList<QString> stationsHash;
    QList<QString> hashList;
    auto stations = App::model()->stationsItem()->model()->items();    
    auto hashArray = obj.value(IPS_STR("listStations")).toArray();

    for (auto i=0; i!=stations.size(); ++i)
        stationsHash.append(stations.value(i)->toHash());
    for (auto hash : hashArray)
        hashList.append(hash.toObject().value(IPS_STR("hashStation")).toString());

    if (!stationsHash.isEmpty()) {
        for (auto i=0; i!=hashList.size(); ++i) {
            for (auto j=0; j!=stationsHash.size(); ++j) {
                if (hashList.value(i) == stations.value(j)->toHash())
                    listStations.append(stations.value(j));
            }
        }
    }    
    return listStations;
}
QJsonArray ModelGroups::serialize() const
{
    QJsonArray res;
    auto items = inherited::items();
    if (!items.isEmpty() && App::model()->stationsItem()->model()->rowCount() && App::model()->groupItem()->groupsIsEmptyChecker()) {
        for (auto item: items) {
            res.push_back(QJsonObject {
              {IPS_STR("name"), item.nameGroupCall()}
            , {IPS_STR("id"), static_cast<int>(item.gid())}
            , {IPS_STR("listStations"), serializeListStations(item)}});
        }
    }
    else {
         QString saveProject;
         saveProject = IPS_STR("SaverProject");
         res.push_back(QJsonObject {{IPS_STR("SaveProjectSettings"), saveProject}});
    }
    return res;
}
bool ModelGroups::deserialize(QJsonArray const& arr)
{       
    for (auto a: arr) {
        if (!a.isUndefined() && a.isObject()) {
            if (!a.toObject().value(IPS_STR("name")).toString().isEmpty()) {
                value_t val;
                val.setNameGroupCall(a.toObject().value(IPS_STR("name")).toString());
                val.setGid(static_cast<gid_t>(a.toObject().value(IPS_STR("id")).toInt()));
                val.setStationList(deserializeListStations(a.toObject()));
                inherited::items().append(val);
            }
        }
        else
            return LOG_ERROR(false, "PairsStations is not an object");
    }
    return true;
}
