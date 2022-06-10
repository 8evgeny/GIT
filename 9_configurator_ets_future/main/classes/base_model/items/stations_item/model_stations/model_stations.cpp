#include "classes/json.h"
#include "classes/defines.h"
#include "classes/cids_container.h"
#include "include/validators/ip_validator.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
	#pragma GCC diagnostic ignored "-Wsign-conversion"
	#pragma GCC diagnostic ignored "-Wold-style-cast"
//	#define PACKED __attribute__ ((__packed__, aligned(1)))
#elif defined(_MSC_VER)
//	#define PACKED
//	#pragma pack(push,1)
#endif

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
	Q_UNUSED(rows);
	unsigned int newPos, newNum;
	bool _res;
	_res = getNumberAndPositionStations(position, &newPos, &newNum);
	if (_res == false)
		return false;
    inherited::beginInsertRows(index, (int)(newPos-1), newPos-1);
    auto item = StationItem::defValue(m_parent, newNum, newNum, IpHelper::next()
                                     , APP_SETTINGS->netmask(), APP_SETTINGS->gateway(), QString::fromLocal8Bit(""));
    item->setModelTones(m_modelTones);
    App::model()->insertItem(newPos, item, stationsItemIndex());
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
		else if (index.column() == 2){
            return role == Qt::EditRole
                    ? QVariant::fromValue<StationType>(item->type())
                    : toString(item->type());
		}
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
	quint32 _maxSupportKey, _newCountKey;
	QMessageBox::StandardButton ret;
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
				 item->setId(convert::toUint(value));
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
			auto itemType = item->type();
			switch(itemType){
				case StationType::PDO16:
				case StationType::PDO16N:
					_maxSupportKey = 16;
				break;
				case StationType::PDO32:
				case StationType::PDO32N:
					_maxSupportKey = 32;
				break;
				case StationType::PDO48:
					_maxSupportKey = 48;
				break;
				case StationType::UPV2:
				case StationType::UPV4:
				case StationType::UPV6:
				case StationType::UPV2N:
				case StationType::UPV4N:
				case StationType::UPV_A:
					_maxSupportKey = 6;
				break;
				default:
					_maxSupportKey = 48;
				break;
			}
            auto type = value.value<StationType>();             
			if (!(type == StationType::Invalid || type == StationType::Unknown)){
				switch(type){
					case StationType::PDO16:
					case StationType::PDO16N:
						_newCountKey = 16;
					break;
					case StationType::PDO32:
					case StationType::PDO32N:
						_newCountKey = 32;
					break;
					case StationType::PDO48:
						_newCountKey = 48;
					break;
					case StationType::UPV2:
					case StationType::UPV4:
					case StationType::UPV6:
					case StationType::UPV2N:
					case StationType::UPV4N:
					case StationType::UPV_A:
						_newCountKey = 6;
					break;
					default:
						_newCountKey = 48;
					break;
				}
				if (_newCountKey < _maxSupportKey){
					ret = QMessageBox::question( m_parent,  QApplication::applicationName(), QObject::tr("buttons with numbers over %1 will be deleted\nAccept new type station? Yes/No").arg(_newCountKey),
										QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
					if (ret == QMessageBox::Yes){
						item->DeleteKeys(_newCountKey+1, _maxSupportKey);
						item->setType(type);
						item->setStationType(type);
					} else {
						return false;
					}
				} else {
					item->setType(type);
					item->setStationType(type);
				}
			}
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
	result.setSN(item->sn());
    return result;
}
QJsonArray ModelStations::serialize() const {    
/*    QJsonArray res;
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
			, {IPS_STR("SN"), item->sn()}
            , {IPS_STR("items"), item->serialize()}
         });
    });
    return res;*/
	QJsonArray res;
	QList<unsigned int> lstCids;
	auto const& items = StationItems();
//create list cids
	for(int _i = 0; _i < items.count(); _i++){
		lstCids.append(items.at(_i)->cid());
	}
//sort
	for(int _i = 0; _i < lstCids.count()-1; _i++){
		if (lstCids.at(_i+1) < lstCids.at(_i))
			lstCids.swapItemsAt(_i, _i+1);
	}
	for(int _i = 0; _i < lstCids.count(); _i++){
		for(int _j = 0; _j < items.count(); _j++){
			if (items.at(_j)->cid() == lstCids.at(_i)){
				QJsonObject obj;
				obj[IPS_STR("hash")] = items.at(_j)->toHash();
				obj[IPS_STR("name")] = items.at(_j)->stationName();
				obj[IPS_STR("cid")] = static_cast<int>(items.at(_j)->cid());
				obj[IPS_STR("type")] = static_cast<int>(items.at(_j)->type());
				obj[IPS_STR("ip")] = items.at(_j)->ip();
				obj[IPS_STR("mask")] = items.at(_j)->stationMask();
				obj[IPS_STR("gateway")] = items.at(_j)->gateway();
				obj[IPS_STR("SN")] = items.at(_j)->sn();
				obj[IPS_STR("items")] = items.at(_j)->serialize();
				res.append(obj);
				break;
			}
		}
	}
	return res;
}

QJsonArray ModelStations::sort_serialize()
{
	QJsonArray res;
	QList<unsigned int> lstCids;
	auto const& items = StationItems();
//create list cids
	for(int _i = 0; _i < items.count(); _i++){
		lstCids.append(items.at(_i)->cid());
	}
//sort
	for(int _i = 0; _i < lstCids.count()-1; _i++){
		if (lstCids.at(_i+1) < lstCids.at(_i))
			lstCids.swapItemsAt(_i, _i+1);
	}
	for(int _i = 0; _i < lstCids.count(); _i++){
		for(int _j = 0; _j < items.count(); _j++){
			if (items.at(_j)->cid() == lstCids.at(_i)){
				QJsonObject obj;
				obj[IPS_STR("hash")] = items.at(_j)->toHash();
				obj[IPS_STR("name")] = items.at(_j)->stationName();
				obj[IPS_STR("cid")] = static_cast<int>(items.at(_j)->cid());
				obj[IPS_STR("type")] = static_cast<int>(items.at(_j)->type());
				obj[IPS_STR("ip")] = items.at(_j)->ip();
				obj[IPS_STR("mask")] = items.at(_j)->stationMask();
				obj[IPS_STR("gateway")] = items.at(_j)->gateway();
				obj[IPS_STR("SN")] = items.at(_j)->sn();
				obj[IPS_STR("items")] = items.at(_j)->serialize();
				res.append(obj);
				break;
			}
		}
	}
	return res;
}

bool ModelStations::deserialize(QJsonArray const& arr)
{
//	qDebug() << QObject::tr("ModelStations::deserialize()");
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

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("SN"), sn)
        item->setSN(sn);

        if(!item->deserialize(a.toObject()))
            return false;
        insertItem(pos++, item.release());
    }
//	qDebug() << QObject::tr("ModelStations::deserialize(), end");
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

bool ModelStations::getNumberAndPositionStations(int currentPos, unsigned int* newPos, unsigned int* Num){
	unsigned int _c_cid;
	unsigned int _next_cid;
	unsigned int _max_cid;
    QList<uint> cids;
//состовляем список cid-ов устройств
	for (int _i = 0; _i < this->rowCount(); _i++){
		cids.append(this->station(_i).cid());
	}
//если нет станций
	if (this->rowCount() == 0){
		*newPos = 1;
		*Num = App::model()->globalParametersItem()->callerid()+1;
		return true;
	}
//считали текущий ключ
	_c_cid = cids.at(currentPos);
//если текущая позиция в конце списка
	if ((currentPos+1) == this->rowCount()){
		*newPos = this->rowCount();
		*Num = _c_cid+1;
		return true;
	}
//если посреди списка смотрим: есть ли впереди следующий номер и если он есть, то равен ли ожидаемому.
//если равен, то вычисляем максимальный номер занятой кнопки и если вставляемая превышает ограничение, то отказ в добавлении
//иначе добавляем в конец списка
	_next_cid = cids.at(currentPos+1);
	if ((_c_cid+1) == _next_cid){
		_max_cid = cids.at(0);
		for(int _i = 1; _i < this->rowCount(); _i++)
			if (cids.at(_i) > _max_cid)
				_max_cid = cids.at(_i);
		*newPos = this->rowCount();
		*Num = _max_cid+1;
		return true;
	}
	*newPos = currentPos+1;
	*Num = _c_cid+1;
	return true;
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
//	#undef PACKED
#elif defined(_MSC_VER)
//	#pragma pack(pop)
//	#undef PACKED
#endif
