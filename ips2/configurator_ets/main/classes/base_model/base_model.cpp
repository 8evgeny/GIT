#include "classes/json.h"
#include <QCryptographicHash>
#include "classes/base_model/items/station_item/model_keys/key.h"
#include "classes/base_model/base_model.h"
#include "classes/base_model/items/root_item.h"
#include "classes/base_model/items/project_info_item/project_info_item.h"
#include "classes/base_model/items/global_parameters_item/global_parameters.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "classes/base_model/items/interfaces/interfaces_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/output_connections_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/input_connections_item.h"

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

BaseModel::BaseModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_root(new RootItem)    
{        
    insertItem(ProjectInfoItemIndex, new ProjectInfoItem(m_root));
    insertItem(GlobalParametersItemIndex, new GlobalParametersItem(m_root));
    insertItem(StationsItemIndex, new StationsItem(m_root));    

//    insertItem(GroupsItemIndex, new GroupsItem(m_root));
//    insertItem(IntrfacesItemIndex, new InterfacesItem(m_root));
//    m_root->child(IntrfacesItemIndex)->insertChild(childsId::SipsItemId, new SipStationsItem(m_root->child(IntrfacesItemIndex)));
//    m_root->child(IntrfacesItemIndex)->child(childsId::SipsItemId)->insertChild(grandChildsId::InputConnectionsItemId, new InputConnectionsItem(m_root->child(IntrfacesItemIndex)->child(childsId::SipsItemId)));
//    m_root->child(IntrfacesItemIndex)->child(childsId::SipsItemId)->insertChild(grandChildsId::OutputConnectionsItemId, new OutputConnectionsItem(m_root->child(IntrfacesItemIndex)->child(childsId::SipsItemId)));
}
BaseModel::~BaseModel()
{
    delete m_root;
}
int BaseModel::columnCount(const QModelIndex&) const
{
    return 1;
}
QVariant BaseModel::data(const QModelIndex &index, int role) const
{
     return (index.isValid() && role == Qt::DisplayRole)
            ? toItem(index)->name()
            : QVariant();
}
Qt::ItemFlags BaseModel::flags(const QModelIndex &index) const
{
    return index.isValid()
            ? QAbstractItemModel::flags(index)
            : Qt::NoItemFlags;
}
QVariant BaseModel::headerData(int, Qt::Orientation, int) const
{
    return {};
}
bool BaseModel::insertItem(int position, BaseItem* item, const QModelIndex& index) noexcept
{
    inherited::beginInsertRows(index, position, position);
    getRoot(index)->insertChild(position, item);
    inherited::endInsertRows();
    return true;
}
void BaseModel::refreshName(const QModelIndex& index, const QVector<int>& roles) noexcept
{
    emit dataChanged(index, index, roles);
}
bool BaseModel::removeRows(int position, int rows, const QModelIndex& index)
{
     inherited::beginRemoveRows(index, position, position + rows - 1);
      for(auto i = 0; i!= rows; ++i)
         getRoot(index)->removeChild(position);
     inherited::endRemoveRows();
     return true;
}
QModelIndex BaseModel::index(int row, int column, const QModelIndex& index) const
{
	QObject::tr("BaseModel::index");
    if (!hasIndex(row, column, index))
        return {};

    auto parent = getRoot(index);
	QObject::tr("BaseModel::index, get root");
    LOG_FATAL(parent, "parent cannot be NULL");
    return parent->child(row)
            ? createIndex(row, column, parent->child(row))
            : QModelIndex();
}
QModelIndex BaseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return {};
    }

    auto parent = toItem(index)->parent();
    return parent != m_root
        ? createIndex(parent->row(), 0, parent)
        : QModelIndex();
}
int BaseModel::rowCount(const QModelIndex& parent) const
{
    return parent.column() > 0
        ? 0
        : getRoot(parent)->childCount();
}
RootItem* BaseModel::root() const noexcept
{
    auto item = dynamic_cast<RootItem*>(m_root);
    LOG_FATAL(item, "Cannot cast to RootItem");
    return item;
}
ProjectInfoItem* BaseModel::projectInfoItem() const noexcept
{
    auto item = dynamic_cast<ProjectInfoItem*>(m_root->child(ProjectInfoItemIndex));
    LOG_FATAL(item, "Cannot cast to ProjectInfoItem");
    return item;
}
GlobalParametersItem* BaseModel::globalParametersItem() const noexcept
{
    auto item = dynamic_cast<GlobalParametersItem*>(m_root->child(GlobalParametersItemIndex));
    LOG_FATAL(item, "Cannot cast to GlobalParametersItem");
    return item;
}
StationsItem* BaseModel::stationsItem() const noexcept
{
    auto item = dynamic_cast<StationsItem*>(m_root->child(StationsItemIndex));
    LOG_FATAL(item, "Cannot cast to StationsItem");
    return item;
}
GroupsItem* BaseModel::groupItem() const noexcept
{
    auto item = dynamic_cast<GroupsItem*>(m_root->child(GroupsItemIndex));
    LOG_FATAL(item, "Cannot cast to GroupsItem");
    return item;
}
SipStationsItem* BaseModel::sipStationsItem() const noexcept
{
    auto item = dynamic_cast<SipStationsItem*>(m_root->child(IntrfacesItemIndex)->child(childsId::SipsItemId));
    LOG_FATAL(item, "Cannot cast to SipStationsItem");
    return item;
}
InterfacesItem* BaseModel::interfacesItem() const noexcept
{
    auto item = dynamic_cast<InterfacesItem*>(m_root->child(IntrfacesItemIndex));
    LOG_FATAL(item, "Cannot cast to InterfacesItem");
    return item;
}
InputConnectionsItem* BaseModel::inputConnectionsItem() const noexcept
{
    auto item = dynamic_cast<InputConnectionsItem*>(sipStationsItem()->child(grandChildsId::InputConnectionsItemId));
    LOG_FATAL(item, "Cannot cast to InputConnectionsItem");
    return item;
}
OutputConnectionsItem* BaseModel::outputConnectionsItem() const noexcept
{
    auto item = dynamic_cast<OutputConnectionsItem*>(sipStationsItem()->child(grandChildsId::OutputConnectionsItemId));
    LOG_FATAL(item, "Cannot cast to OutputConnectionsItem");
    return item;
}
bool BaseModel::itemClick(const QModelIndex& index, QWidget* content, QWidget* settings)
{    
	QObject::tr("BaseModel::itemClick");
	if (m_current){
        m_current->deshow();
		QObject::tr("BaseModel::itemClick, m_current->deshow()");
	}
    if (!index.isValid())
        return false;    
    m_current = toItem(index)->show(content, settings);
    return true;
}
void BaseModel::selectProjectInfo(QWidget* content, QWidget* settings)
{
    App::model()->itemClick(App::model()->index(0, 0), content, settings);
}
QVariantMap BaseModel::dataSerialize() const noexcept
{
    auto dlg = App::messages();
    LOG_FATAL(dlg, "dlg can not be nullptr");

    QVariantMap map;
    map.insert(IPS_STR("version"), 1);
    map.insert(IPS_STR("Project information"), m_root->items().value(ProjectInfoItemIndex)->serialize());
    map.insert(IPS_STR("Global parameters"), m_root->items().value(GlobalParametersItemIndex)->serialize());
    map.insert(IPS_STR("Stations"), m_root->items().value(StationsItemIndex)->serialize());
//===отключены=====
//    map.insert(IPS_STR("Groups"), m_root->items().value(GroupsItemIndex)->serialize());
//    map.insert(IPS_STR("Interfaces"), m_root->items().value(IntrfacesItemIndex)->serialize());
//    map.insert(IPS_STR("InputConnections"), inputConnectionsItem()->serialize());
//    map.insert(IPS_STR("OutputConnections"), outputConnectionsItem()->serialize());
//    map.insert(IPS_STR("Messages"), dlg->model()->serialize());
    return map;
}
QByteArray BaseModel::compressData(QJsonDocument const& doc) const noexcept
{
    if (!doc.isEmpty())
        return qCompress(doc.toJson(), 9);
    return  {};
}
QJsonDocument BaseModel::document() const noexcept
{    
    QJsonDocument doc;
    doc.setObject(QJsonObject::fromVariantMap(dataSerialize()));    
    return  doc;
}
QByteArray BaseModel::toHash() noexcept
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!compressData(document()).isEmpty()) {
       hash.addData(compressData(document()));
       return hash.result();
    }
    return {};
}
bool BaseModel::save(QString const& file) const
{
    return json::save(document(), file);
}
bool BaseModel::load(QString const& file)
{
    auto const& json = json::load(file).object();
    LOG_FATAL(!json.isEmpty(), "Invalid json");
    if (json.isEmpty())
        return false;

    if (json.value(IPS_STR("version")).toInt() != 1) {
        LOG_FATAL(!json.isEmpty(), "Version %1 not supported", json.value(IPS_STR("version")));
        return false;
    }
    selectProjectInfo(m_content, m_settings);
	for(int _i = 0; _i < m_root->items().count(); _i++){
//		qDebug() << QObject::tr("StationsItem::load(), end begin cicl, _i:%1").arg(_i);
//		qDebug() << QObject::tr("StationsItem::load(), m_root->items().at(_i)->clear(), _i:%1").arg(_i);
		m_root->items().at(_i)->clear();
//		qDebug() << QObject::tr("StationsItem::load(), m_root->items().at(_i)->deserialize(json), _i:%1").arg(_i);
		m_root->items().at(_i)->deserialize(json);
//		qDebug() << QObject::tr("StationsItem::load(), end for cicl");
	}
//    for (auto const& item: m_root->items()) {
//        item->clear();
//        item->deserialize(json);
//    }
//    App::model()->sipStationsItem()->clear();
//    App::model()->inputConnectionsItem()->clear();
//    App::model()->outputConnectionsItem()->clear();
//    IPS_JSON_EXTRACT_OBJECT(json, data, IPS_STR("Interfaces"))
//    IPS_JSON_EXTRACT_OBJECT(data.toObject(), item, IPS_STR("SipStations"))
//    App::model()->sipStationsItem()->deserialize(item.toObject());
//    App::model()->inputConnectionsItem()->deserialize(json);
//    App::model()->outputConnectionsItem()->deserialize(json);
	QObject::tr("StationsItem::load(), selectProjectInfo");
    selectProjectInfo(m_content, m_settings);
    return true;
}
QString BaseModel::getConfigureInfo()
{
	QVariantMap map;
	QString _result;
	QJsonDocument doc;
	map.insert(IPS_STR("version"), 1);
	map.insert(IPS_STR("Global parameters"), m_root->items().value(GlobalParametersItemIndex)->serialize());
//	map.insert(IPS_STR("Groups"), m_root->items().value(GroupsItemIndex)->serialize());
	map.insert(IPS_STR("Stations"), m_root->items().value(StationsItemIndex)->serialize());
	doc.setObject(QJsonObject::fromVariantMap(map));
	_result = QString::fromLocal8Bit(doc.toJson());
	return _result;
}
template<class T, class F>
T BaseModel::getListCids(F f) noexcept
{
    T list;
    auto const& stations = stationsItem();
    for (auto i = 0; i != stations->childCount(); ++i) {
        auto item = dynamic_cast<StationItem*>(stations->child(i));
        LOG_FATAL(item, "Cannot cast to StationItem");
        list << f(item);
    }
    return list;
}
template<class T, class F>
T BaseModel::getListGids(F f) noexcept
{
    T list;
    auto const& groupCalls = groupItem()->group()->modelGroups()->items();
    for (auto i = 0; i != groupCalls.size(); ++i) {
        auto item = groupCalls.value(i);
        list << f(&item);
    }
    return list;
}
template<class T, class F>
T BaseModel::getListOids(F f) noexcept
{
    T list;
    auto const& connections = outputConnectionsItem();
    auto const& items = connections->model()->items();
    for (auto i = 0; i != items.size(); ++i) {
        auto item = items.value(i);
        list << f(&item);
    }
    return list;
}
template<class T>
T BaseModel::getListKeys(StationItem* station) noexcept
{
    T list;
    for (auto const& item : station->model().keys())
         list.append(item.id());
    return list;
}
key_list_t BaseModel::keys(StationItem* station) noexcept
{
    return getListKeys<key_list_t>(station);
}
cid_list_t BaseModel::cids() noexcept
{
    return getListCids<cid_list_t>([](StationItem* item) -> cid_t {
        return item->cid();
    });
}
oid_list_t BaseModel::oids() noexcept
{
    return getListOids<oid_list_t>([](OutputConnection* item) -> oid_t {
        return item->oid();
    });
}
gid_list_t BaseModel::gids() noexcept
{
    return getListGids<gid_list_t>([](GroupCall* item) -> gid_t {
        return item->gid();
    });
}
BaseModel::ip_list_t BaseModel::ips() noexcept
{
    return getListCids<ip_list_t>([](StationItem* item) -> QString {
        return item->ip();
    });
}
BaseModel::ip_list_t BaseModel::netmasks() noexcept
{
    return getListCids<ip_list_t>([](StationItem* item) -> QString {
        return item->ip();
    });
}
BaseModel::ip_list_t BaseModel::gateways() noexcept
{
    return getListCids<ip_list_t>([](StationItem* item) -> QString {
        return item->ip();
    });
}
gr_list_t BaseModel::grs() noexcept
{
    gr_list_t list;
    std::sort(list.begin(), list.end());
    return list;
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