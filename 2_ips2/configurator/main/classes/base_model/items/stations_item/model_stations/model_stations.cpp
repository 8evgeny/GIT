#include "classes/json.h"
#include "classes/defines.h"
#include "classes/cids_container.h"
#include "include/validators/ip_validator.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"

ModelStations::ModelStations(QObject* parent)
    : inherited(parent)
{
}
QModelIndex ModelStations::stationsItemIndex() const noexcept
{
    return App::model()->index(BaseModel::StationsItemId, 0, {});
}
int ModelStations::rowCount(const QModelIndex&) const
{   
    return App::model()->stationsItem()->childCount();
}
int ModelStations::columnCount(const QModelIndex&) const
{
    return 4;
}
Qt::ItemFlags ModelStations::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return  QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool ModelStations::insertRows(int position, int rows, const QModelIndex& index)
{
    inherited::beginInsertRows(index, position, position + rows - 1);
    for(auto i = 0; i!= rows; ++i) {
        auto item = StationItem::defValue(m_parent, CidHelper::next(), CidHelper::next(), IpHelper::next()
                                          , APP_SETTINGS->netmask(), APP_SETTINGS->gateway());
        item->setModelTones(m_modelTones);
        App::model()->insertItem(position + i, item, stationsItemIndex());
    }
    inherited::endInsertRows();
    return true;
}
bool ModelStations::insertItem(int position, StationItem* item,  QModelIndex const& index)
{
    inherited::beginInsertRows(index, position, position);
    App::model()->insertItem(position, item, stationsItemIndex());
    inherited::endInsertRows();
    return true;
}
bool ModelStations::removeRows(int position, int rows, const QModelIndex& index) {
     inherited::beginRemoveRows(index, position, position + rows - 1);
     for(auto i = 0; i != rows; ++i) {
         auto item = dynamic_cast<StationItem*>(App::model()->stationsItem()->child(position));
         auto& cids = App::model()->globalParametersItem()->cids();
         QString hash = item->toHash();
         auto pos = CidsContainer::findPosCid(cids, hash);
         if (pos < cids.size())
            cids.removeAt(pos);
         emit(removeStation(hash));
         App::model()->removeRow(position, stationsItemIndex());         
     }
     inherited::endRemoveRows();
     return true;
}
QVariant ModelStations::data(const QModelIndex& index, int role) const
{    
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return {};

    auto item = dynamic_cast<StationItem*>(App::model()->stationsItem()->child(index.row()));
    LOG_FATAL(item, "StationItem cannot be NULL");

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return item->cid();
        else if (index.column() == 1)
            return convert::truncate(item->stationName(), 64);
        else if (index.column() == 2)
            return role == Qt::EditRole
                    ? QVariant::fromValue<StationType>(item->type())
                    : toString(item->type());
        else if (index.column() == 3)
            return item->ip();       
    }
    return {};
}
QVariant ModelStations::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return IPS_STR("CID");
            case 1:
                return iStation::tr("Name");
            case 2:
                return iStation::tr("Type");
            case 3:
                return IPS_STR("IP");
        }
     }
    return QVariant();
}
bool ModelStations::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto item = dynamic_cast<StationItem*>(App::model()->stationsItem()->child(index.row()));
        LOG_FATAL(item, "StationItem cannot be NULL");
        if (index.column() == 0) {
             QRegularExpression reg;
             reg.setPattern(IPS_STR("^[0]"));
             if (isCid(value) && !CidHelper::contains(toCid(value)) && !reg.match(value.toString()).hasMatch()) {
                 item->setCid(toCid(value));
                 updateCid(item);
                 refreshStationItemName(index.row());
             }
        }
        if (index.column() == 1) {
             if (!value.toString().isEmpty()) {                 
                 item->setStationName(convert::truncate(value.toString(), 64));
                 refreshStationItemName(index.row());
             }
        }
        else if (index.column() == 2) {
             auto type = value.value<StationType>();             
             if (!(type == StationType::Invalid || type == StationType::Unknown))
                 item->setType(type);
        }
        else if (index.column() == 3) {
             if (IpValidator::isValid(value) && !IpHelper::contains(value)) {
                     item->setIp(value.toString());                    
             }
        }
    }
    emit(dataChanged(index, index, QVector<int>{Qt::EditRole}));
    return true;
}
void ModelStations::refreshStationItemName(int row) const noexcept
{
     App::model()->refreshName(App::model()->index(0, row + 1
                               , App::model()->index(0, BaseModel::StationsItemId)));
}
QList<StationItem*> ModelStations::StationItems() const noexcept
{
    QList<StationItem*> result;
    auto const& items = App::model()->stationsItem()->items();
    std::transform(items.cbegin(), items.cend(), std::back_inserter(result), [] (auto item) {
        auto val = dynamic_cast<StationItem*>(item);
        LOG_FATAL(val, "Cannot cast to StationItem");
        return val;
    });
    return result;
}
void ModelStations::updateItem(int row, Station const& val, const QVector<int>& roles) noexcept
{      
    auto item = dynamic_cast<StationItem*>(App::model()->stationsItem()->child(row));
    LOG_FATAL(item, "StationItem cannot be NULL");
    if (val.cid() != 0) {
        item->setCid(val.cid());
        updateCid(item);
    }
    item->setStationName(val.stationName());
    item->setType(val.type());
    item->setIp(val.ip());    
    item->setStationMask(val.stationMask());
    item->setGateway(val.gateway());
    emit dataChanged(index(row, 0), index(row, columnCount()), roles);
    refreshStationItemName(row);
}
Station ModelStations::station(int row) const noexcept
{
    auto item = dynamic_cast<StationItem*>(App::model()->stationsItem()->child(row));
    LOG_FATAL(item, "StationItem cannot be NULL");
    Station result;
    result.setCid(item->cid());
    result.setIp(item->ip());
    result.setId(item->id());
    result.setStationMask(item->stationMask());
    result.setStationName(item->stationName());
    result.setType(item->type());
    result.setGateway(item->gateway());
    return result;
}
QJsonArray ModelStations::serialize() const {    
    QJsonArray res;
    auto const& items = StationItems();
    std::transform(items.cbegin(), items.cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
              {IPS_STR("hash"), item->toHash()}
            , {IPS_STR("name"), item->stationName()}
            , {IPS_STR("cid"), static_cast<int>(item->cid())}
            , {IPS_STR("type"), static_cast<int>(item->type())}
            , {IPS_STR("ip"), item->ip()}
            , {IPS_STR("mask"), item->stationMask()}
            , {IPS_STR("gateway"), item->gateway()}
            , {IPS_STR("items"), item->serialize()}
         });
    });
    return res;
}
bool ModelStations::deserialize(QJsonArray const& arr)
{
    auto pos = 0;
    for(auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = std::make_unique<StationItem>(m_parent);
        IPS_JSON_TO_STR(a.toObject(), IPS_STR("hash"), hash)
        item->setHash(hash);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("name"), name)
        item->setStationName(name);

        IPS_JSON_TO_UINT(a.toObject(), IPS_STR("cid"), cid)
        item->setCid(cid);

        IPS_JSON_TO_INT(a.toObject(), IPS_STR("type"), type)
        item->setType(static_cast<StationType>(type));

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("ip"), ip)
        item->setIp(ip);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("mask"), mask)
        item->setStationMask(mask);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("gateway"), gateway)
        item->setGateway(gateway);

        if(!item->deserialize(a.toObject()))
            return false;

        insertItem(pos++, item.release());
    }
    return true;
}
void ModelStations::updateCid(item_t item) noexcept
{
    QString hash = item->toHash();
    auto& cids = App::model()->globalParametersItem()->cids();
    auto cid = item->cid();
    auto pos = CidsContainer::findPosCid(cids, hash);
    if (pos < cids.size()) {
        auto contCid = cids.value(pos);
        LOG_FATAL(contCid, "contCid can not be nulptr");
        contCid->setCid(cid);
    }
}
