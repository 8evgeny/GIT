#include "model_keys.h"
#include "classes/json.h"
#include "classes/defines.h"
#include "classes/base_model/items/station_item/model_keys/key_helper.h"

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
	int newPos, newNum;
	bool _res;
	_res = getNumberAndPositionKey(position, &newPos, &newNum);
	if (_res == false)
		return false;
    if(rows < 1)
        return false;
    inherited::beginInsertRows(index, newPos-1, newPos-1);
    inherited::items().insert(newPos, *defValue(newNum));
    inherited::endInsertRows();
	m_row = newPos;
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
	unsigned int currentMaxKey;
	switch(stType){
		case StationType::PDO16:
		case StationType::PDO16N:
			currentMaxKey = 16;
			break;
		case StationType::PDO32:
		case StationType::PDO32N:
			currentMaxKey = 32;
			break;
		case StationType::PDO48:
			currentMaxKey = 48;
			break;
		case StationType::UPV2:
		case StationType::UPV4:
		case StationType::UPV6:
		case StationType::UPV2N:
		case StationType::UPV4N:
		case StationType::UPV_A:
			currentMaxKey = 6;
		break;
		default:
			currentMaxKey = 48;
			break;
	}
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& key = inherited::item(index.row());
        auto const& val = value.toString();

        if (index.column() == 0) {
//			qDebug() << currentMaxKey;
			if (isKey(value) && !KeyHelper::contains(value.toUInt(), m_parent) && value.toInt() !=0 && (value.toUInt() <= currentMaxKey)){
                key.setId(value.toUInt());
			}
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

bool ModelKeys::getNumberAndPositionKey(int currentPos, int* newPos, int* Num){
//	int _c_key;
//	unsigned int _next_key;
//	unsigned int _max_key;
//	unsigned int currentMaxKey;
//если не один ключ не задан
	if (this->rowCount() == 0){
		*newPos = 1;
		*Num = 1;
		return true;
	}
//ищем свободный номер кнопки
	if (!getFreeNumberKey(true, currentPos, newPos, Num)){
		if (!getFreeNumberKey(false, currentPos, newPos, Num))
			return false;
	}
/*
//если текущая позиция в конце списка
	if ((currentPos+1) == this->rowCount()){
		*newPos = this->rowCount();
		*Num = _c_key;
		return true;
	}

	int _c_key = keys().at(currentPos).id();
	if (_c_key == getMaximumKeyNumber())
		return false;
	if (_isDownFind){
		for (int _i = _startPos+1; _i < keys().count(); _i++){
			if (keys().at(_i).id() == (c_num+1)){
				c_num = keys().at(_i).id();
			} else {
				*pNumber = c_num+1;
				return true;
			}
		}
		if (keys().at(keys().count()-1).id() == getMaximumKeyNumber()){
			return false;
		} else {
			*pNumber = keys().at(keys().count()-1).id()+1;
			return true;
		}
	} else {
	}
*/



/*
//считали текущий ключ
	_c_key = keys().at(currentPos).id();
//если текущая позиция в конце списка
	if ((currentPos+1) == this->rowCount()){
		*newPos = this->rowCount();
		*Num = _c_key+1;
		return true;
	}
//если посреди списка смотрим: есть ли впереди следующий номер и если он есть, то равен ли ожидаемому.
//если равен, то вычисляем максимальный номер занятой кнопки и если вставляемая превышает ограничение, то отказ в добавлении
//иначе добавляем в конец списка
	_next_key = keys().at(currentPos+1).id();
	if ((_c_key+1) == _next_key){
		_max_key = keys().at(0).id();
		for(int _i = 1; _i < this->rowCount(); _i++)
			if (keys().at(_i).id() > _max_key)
				_max_key = keys().at(_i).id();
		if (_max_key == currentMaxKey){
			return false;
		}
		*newPos = this->rowCount();
		*Num = _max_key+1;
		return true;
	}
	*newPos = currentPos+1;
	*Num = _c_key+1;*/
	return true;
}

void ModelKeys::swapRow(int srcRowPos, int dstRowPos)
{
	inherited::items().swapItemsAt(srcRowPos, dstRowPos);
}

int ModelKeys::idKey(int row)
{
	return inherited::items().at(row).id();
}

void ModelKeys::updateKey(int row, const Key& key)
{
	setItem(row, key);
}

int ModelKeys::getCurrentRow()
{
	return m_row;
}

bool ModelKeys::getFreeNumberKey(bool _isDownFind, int _startPos, int* _newPos, int* _pNumber)
{
	unsigned int c_num = keys().at(_startPos).id();
	if (_isDownFind){
//поиск "вниз"
		for (int _i = _startPos+1; _i < keys().count(); _i++){
			if (keys().at(_i).id() == (c_num+1)){
				c_num = keys().at(_i).id();
			} else {
				*_pNumber = c_num+1;
				*_newPos = _i;
				return true;
			}
		}
		if (keys().at(keys().count()-1).id() == getMaximumKeyNumber()){
			return false;
		} else {
			*_pNumber = keys().at(keys().count()-1).id()+1;
			*_newPos = keys().count();
			return true;
		}
	} else {
//поиск "вверх"
		qDebug() << QString::fromLocal8Bit("search up\n");
		for (int _i = _startPos-1; _i >= 0; _i--){
//			qDebug() << QObject::tr("_i = %1, id = %2, c_num = %3\n").arg(_i).arg(keys().at(_i).id()).arg(c_num);
			if (keys().at(_i).id() == (c_num-1)){
				c_num = keys().at(_i).id();
			} else {
				*_pNumber = c_num-1;
				*_newPos = _i+1;
				return true;
			}
		}
		if (c_num == 1)
				return false;
		*_newPos = 1;
		*_pNumber = c_num-1;
//		qDebug() << QObject::tr("_newPos = %1, _pNumber = %2\n").arg(*_newPos).arg(*_pNumber);
		return true;
	}
	return false;
}

unsigned int ModelKeys::getMaximumKeyNumber()
{
	switch(stType){
		case StationType::PDO16:
		case StationType::PDO16N:
			return 16;
			break;
		case StationType::PDO32:
		case StationType::PDO32N:
			return 32;
			break;
		case StationType::PDO48:
			return 48;
			break;
		case StationType::UPV2:
		case StationType::UPV4:
		case StationType::UPV6:
		case StationType::UPV2N:
		case StationType::UPV4N:
		case StationType::UPV_A:
			return 6;
		break;
		default:
			return 48;
			break;
	}
	return 0;
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
