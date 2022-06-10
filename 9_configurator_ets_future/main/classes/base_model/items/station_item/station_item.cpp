#include "include/view.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "ui_station_item.h"
#include <QCryptographicHash>
#include "station_item_instance.h"
#include "dialogs/type_list/type_list.h"
#include "classes/base_types/station_types.h"
#include "classes/base_types/function_types.h"
#include "dialogs/limit_rows/limitrows.h"
#include "dialogs/message_box/message_box.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/station_item/model_keys/key_helper.h"
#include "classes/base_model/items/groups/groups_item.h"
#include "main/messages/model_tracks/tracks_model.h"

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

StationItem::StationItem(BaseItem* item)
    : inherited(item)
{
    m_model.setParentStationItem(this);
    m_isChecked = false;    
    m_settings.setModelTones(m_modelTones);
	m_parent = item;
}
StationItem::~StationItem()
{
    delete ui;
}
void StationItem::updaterGroupsList(itemsGroups_t const& groups)
{
    ips::algo::clearList<>(m_groupsList);
    for (auto const& group : groups) {
        auto name = group.nameGroupCall();
        m_groupsList.append(name);
    }
}
StationItem::class_name* StationItem::defValue(BaseItem* parent, StationType type, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway, QString const& sn)
{
    auto result = new StationItem(parent);
    result->setIp(ip);    
	result->setSN(sn);
    result->setId(id);
    result->setType(type);
    result->setStationMask(mask);
//    result->setStationName(GT_STR("NewStation").arg(cid));
    result->setStationName(GT_STR("NewStation"));
    result->setGateway(gateway);
    result->setCid(cid);
    QByteArray arr;
    arr.append(static_cast<char>(result->id()));
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(arr);
    QString hashStr;
    QLatin1String hashLatin(hash.result());
    hashStr.append(hashLatin);
    result->setHash(hashStr);
    auto& cids = App::model()->globalParametersItem()->cids();
    cids.append(new CidsContainer(hashStr, cid));
	result->setStationType(type);
    return result;
}
StationItem::class_name* StationItem::defValue(BaseItem* parent, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway, QString const& sn)
{
    return defValue(parent, StationType::PDO16, cid, id, ip, mask, gateway, sn);
}
void StationItem::instanceConnectionToSettings()
{
    QObject::connect(&m_settings, SIGNAL(changedCid(cid_t)), this, SLOT(changedCid(cid_t)));
    QObject::connect(&m_settings, SIGNAL(stationChanged(const Station&)), this, SLOT(stationChanged(const Station&)));
	QObject::connect(&m_settings, SIGNAL(keyChanged(Key&, bool)), this, SLOT(keyChanged(Key&, bool)));    
	QObject::connect(&m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)));    
}

void StationItem::sortTableKey()
{
	for(int _i = 0; _i < m_model.rowCount(); _i++){
		for (int _j = 0; _j < m_model.rowCount()-1; _j++)
			if (m_model.keys().at(_j).id() > m_model.keys().at(_j+1).id()){
				m_model.swapRow(_j, _j+1);
			}
	}
}

void StationItem::selectedRowDelete() noexcept
{
    auto const& indx = mh::selected(ui->m_table);
	if (!indx.isValid()){
        return;
	}
	if (indx.row() < 0){
		qDebug() << QString::fromLocal8Bit("row out of range");
		return;
	}
    m_model.removeRow(indx.row());
    mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()));

    if (m_row >= m_model.rowCount())
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()-1));
}
void StationItem::keysSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    auto const& indx = mh::selected(ui->m_table);
    mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()));

    if (m_row >= m_model.rowCount())
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()-1));
}
void StationItem::instanceSignals()
{    
    ui->m_table->setContextMenuPolicy(Qt::CustomContextMenu);
//    auto groupsItem = App::model()->groupItem();
//    LOG_FATAL(groupsItem, "groupsItem can not be nullptr");
//    QObject::connect(groupsItem, SIGNAL(updateGroupsList(itemsGroups_t const&)), this, SLOT(updaterGroupsList(itemsGroups_t const&)));
    QObject::connect(ui->m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(keysSelectionChanged(const QItemSelection&,const QItemSelection&)));
    QObject::connect(ui->m_table, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableview_customContextMenuRequested(const QPoint &)));
    QObject::connect(ui->m_table, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tableview_Clicked(const QModelIndex&)));
    QObject::connect(ui->m_table, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(tableview_doubleClicked(const QModelIndex&)));
	QObject::connect(ui->m_add, SIGNAL(clicked()), this, SLOT(m_add_clicked()));
    QObject::connect(ui->m_rem, SIGNAL(clicked()), this, SLOT(m_rem_clicked()));  
}

void StationItem::on_sectionClicked ( int logicalIndex,  Qt::SortOrder order )
{
	ui->m_table->sortByColumn(logicalIndex, order);
}

void StationItem::changeEvent(QEvent* event)
{    
    if (event->type() == QEvent::LanguageChange) {
        if (ui) {
            ui->retranslateUi(this);
            m_settings.retranslateUi();
        }
    }
    QWidget::changeEvent(event);
}
StationItem* StationItem::show(QWidget* content, QWidget* settings)
{    
    ips::algo::removeChildren(content);
    ips::algo::removeChildren(settings);
    ips::algo::createContent(ui, content);

    if (m_model.rowCount() == MAX_COUNT_KEYS)
        ui->m_add->setDisabled(true);

    m_settings.setupUi(settings, this, this);

    if (!m_model.rowCount())
        m_settings.hideSettings(false);

    ui->m_table->setModel(&m_model);
	m_model_proxy.setSourceModel(&m_model);
	ui->m_table->setModel(&m_model_proxy);
	ui->m_table->setSortingEnabled(true);
    ui->m_table->setShowGrid(false);
    ui->m_table->setContextMenuPolicy(Qt::CustomContextMenu);
	//===сортировка по колонке===
	QHeaderView* m_horiz_header = ui->m_table->horizontalHeader();
	connect(m_horiz_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),  this, SLOT(on_sectionClicked(int, Qt::SortOrder)));
	ui->m_table->sortByColumn(0, Qt::AscendingOrder);
    instanceSignals();
    instanceConnectionToSettings();
    StationItemInstance(ui);

   if(m_row == -1 || m_model.rowCount() > 0)
        m_row = 0;
    mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_row);
    ui->m_rem->setEnabled(m_model.rowCount());
    if (m_model.rowCount())
        m_settings.setKeySettingsEnabled(true, m_model.rowCount());
    else
        m_settings.setKeySettingsEnabled(false, m_model.rowCount());
    return this;
}
void StationItem::deshow()
{
	QObject::tr("StationItem::deshow()");
    QObject::disconnect(ui->m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(keysSelectionChanged(const QItemSelection&,const QItemSelection&)));
	QObject::disconnect(ui->m_table, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableview_customContextMenuRequested(const QPoint &)));
	QObject::disconnect(ui->m_table, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tableview_Clicked(const QModelIndex&)));
	QObject::disconnect(ui->m_table, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(tableview_doubleClicked(const QModelIndex&)));
	QObject::disconnect(ui->m_add, SIGNAL(clicked()), this, SLOT(m_add_clicked()));
	QObject::disconnect(ui->m_rem, SIGNAL(clicked()), this, SLOT(m_rem_clicked()));  
    QObject::disconnect(&m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)));    
	QObject::disconnect(&m_settings, SIGNAL(changedCid(cid_t)), this, SLOT(changedCid(cid_t)));
    QObject::disconnect(&m_settings, SIGNAL(stationChanged(const Station&)), this, SLOT(stationChanged(const Station&)));
	QObject::disconnect(&m_settings, SIGNAL(keyChanged(Key&, bool)), this, SLOT(keyChanged(Key&, bool)));    
    delete ui;
    ui = nullptr;
}
bool StationItem::error(BaseItem*) const
{
    return true;
}
QString StationItem::name(BaseItem*) const
{    
    return IPS_STR("%1(%2)").arg(stationName(), cidToString(cid()));
}
bool StationItem::clear()
{
    return true;
}
QJsonValue StationItem::serialize() const
{
    return QJsonObject{{IPS_STR("activestation"), m_model.serialize()}};
}
bool StationItem::deserialize(QJsonObject const& data)
{
    IPS_JSON_EXTRACT_OBJECT(data, items, IPS_STR("items"))
    IPS_JSON_EXTRACT_ARRAY(items.toObject(), astations, IPS_STR("activestation"))
    return m_model.deserialize(astations.toArray());
}

void StationItem::DeleteKeys(int min, int max)
{
	Q_UNUSED(min);
	Q_UNUSED(max);
	int _count = m_model.rowCount();
	for(int _i = 0; _i < _count; _i++){
		for(int _j = 0; _j < m_model.rowCount(); _j++){
			if ((m_model.idKey(_j) >= min) && (m_model.idKey(_j) <= max)){
				m_model.removeRow(_j);
				break;
			}
		}
	}
}

int StationItem::getMaxSupportKey(StationType type)
{
	switch(type){
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
	return 48;
}

void StationItem::insertKey(int row)
{
	Q_UNUSED(row);
	ui->m_add->clicked();
}
void StationItem::tableview_customContextMenuRequested(const QPoint &)
{
        QMenu menu;
        auto add = menu.addAction(QIcon(IPS_STR(":/svg/plus_qmenu_dark.svg")), iMenu::tr("Add key"), [this]() {
            ui->m_add->clicked();
            m_settings.hideSettings(true);
        });
        auto rem = menu.addAction(QIcon(IPS_STR(":/svg/minus_qmenu_dark.svg")), iMenu::tr("Remove key"), [this]() {
            ui->m_rem->clicked();            
            if (!m_model.rowCount())
               m_settings.hideSettings(false);
        });
        menu.addSeparator();
        auto remAll = menu.addAction(QIcon(IPS_STR(":/svg/basket_qmenu_dark.svg")), iMenu::tr("Remove all Keys"), [this]() {
            m_model.removeRows(0, m_model.rowCount());
            ui->m_rem->setEnabled(m_model.rowCount());
            ui->m_add->setEnabled(true);
            m_settings.hideSettings(false);
        });
        add->setEnabled((m_model.rowCount() < MAX_COUNT_KEYS) ? true : false);
        rem->setEnabled(m_model.rowCount());
        remAll->setEnabled(m_model.rowCount());
        menu.exec(QCursor::pos());
}
void StationItem::tableview_Clicked(const QModelIndex& index)
{
        if (!index.isValid())
            return;

        m_row = index.row();        
        auto key = m_model.item(m_row);        
        m_settings.setKey(key);                
        mh::simple::view::selectRow(ui->m_table, index.row());
        m_settings.hideSettings(true);
        m_settings.setSettingsPretone();
}
void StationItem::tableview_doubleClicked(const QModelIndex& index)
{
	QStringList _cidList;
        if (!index.isValid())
            return;

        auto key = m_model.item(m_row);
        if (index.column() == 2) {
            TypeListDialog<function_type::items_t> dlg(iFunction::tr("Functions"), function_type::defaultFunctionTypes(), key.function());
            if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {
                key.setFunction(dlg.current());
                key.changePriority(dlg.current());
                key.setNumberFunction(dlg.current());
                m_model.setItem(mh::selected(ui->m_table).row(), key, {Qt::EditRole});
            }
        }
        if (index.column() == 3) {
//            TypeListDialog<QStringList> dlg(iFunction::tr("Destiations"), cidGtList(), key.destination());
		    auto const& cids = App::model()->cids();
		    std::transform(cids.cbegin(), cids.cend(), std::back_inserter(_cidList), [] (auto cid) {
				return cidToString(cid);
			});
            TypeListDialog<QStringList> dlg(iFunction::tr("Destiations"), _cidList, key.destination());
           if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {               
                 key.setDestination(dlg.current());
                m_model.setItem(mh::selected(ui->m_table).row(), key, {Qt::EditRole});
            }
        }
}
void StationItem::m_add_clicked()
{
		m_model.setStationType(type());
//считываем тип станции
		StationType _type = type();
//вычисляем максимальное количество кнопок станции
		switch(_type){
			case StationType::PDO16:
			case StationType::PDO16N:
				currentMaxKey = 16;
				break;
			case StationType::PDO32:
			case StationType::PDO32N:
				currentMaxKey = 32;
				break;
			case StationType::PDO48:
				currentMaxKey = MAX_COUNT_KEYS;
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
				currentMaxKey = MAX_COUNT_KEYS;
				break;
		}
//если число кнопок уже максимальное - отказ
		if (m_model.rowCount() == currentMaxKey) {
            ui->m_add->setEnabled(false);
            LimitRowsDialog* dlg = new LimitRowsDialog(this, currentMaxKey);
            dlg->exec();
			delete dlg;
			return;
        }
		bool res = m_model.insertRows(m_row, 1);
//        m_model.insertRows(m_model.rowCount(), 1);
		if (res)
			ui->m_rem->setEnabled(true);
		if (res == false){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Refusal to add a button"));
		}
		sortTableKey();
		ui->m_table->sortByColumn(0, Qt::DescendingOrder);
		ui->m_table->sortByColumn(0, Qt::AscendingOrder);
        m_settings.setKeySettingsEnabled(true, m_model.rowCount());
//        mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.items().size()-1);
		mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.getCurrentRow());
//        auto const& indx = mh::selected(ui->m_table);
//        m_defaultValues.insert(indx.row(), false);
        m_settings.hideSettings(true);
		if (m_model.rowCount() == currentMaxKey) {
            ui->m_add->setEnabled(false);
        }
}
void StationItem::m_rem_clicked()
{
	ui->m_add->setEnabled(true);
//	qDebug() << QObject::tr("row0");
    auto const& indx = mh::selected(ui->m_table);
	if (!indx.isValid()){
		return;
	}
//	qDebug() << QObject::tr("row1=%1, m_defaultValues.count=%1").arg(indx.row()).arg(m_defaultValues.count());
//	m_defaultValues[indx.row()] = true;
//	qDebug() << QObject::tr("row2");
    selectedRowDelete();

	if (!m_model.rowCount())
		m_settings.hideSettings(false);

	if (!m_model.rowCount()) {
		ui->m_rem->setEnabled(false);           
		m_settings.setKeySettingsEnabled(false, m_model.rowCount());
	}
}
void StationItem::dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles)
{
		if (index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole){
            m_settings.setKey(m_model.item(index.row()));
			sortTableKey();
			ui->m_table->sortByColumn(0, Qt::DescendingOrder);
			ui->m_table->sortByColumn(0, Qt::AscendingOrder);
//			mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.items().size()-1);
		}
}
void StationItem::changedCid(cid_t cidStation)
{
	if (cidStation != static_cast<uint>(cid()))
        m_settings.setCidStatus(true);
}
void StationItem::stationChanged(Station const& station)
{
	QMessageBox::StandardButton ret;
	int c_maxkey, n_maxkey;
	m_type = type();
	StationType n_type = station.type();
	if (m_type != n_type){
		c_maxkey = getMaxSupportKey(m_type);
		n_maxkey = getMaxSupportKey(n_type);
//	   qDebug() << (int)m_type << (int)n_type;
		if (n_maxkey < c_maxkey){
			ret = QMessageBox::question( this,  
									QApplication::applicationName(), 
									QObject::tr("buttons with numbers over %1 will be deleted\nAccept new type station? Yes/No").arg(n_maxkey),
									QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
			if (ret == QMessageBox::No){
				m_settings.setType(m_type);
				return;
			}
		}
	}
//		mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.items().size()-1);
	DeleteKeys(getMaxSupportKey(station.type()), MAX_COUNT_KEYS);
       setStationName(station.stationName());
       setType(station.type());
       if (!IpHelper::contains(station.ip()))
           setIp(station.ip());
      setStationMask(station.stationMask());
       setGateway(station.gateway());
	setSN(station.sn());
		if (station.cid() != 0 && !CidHelper::contains(station.cid())){
            setCid(station.cid());
		}
        App::model()->tree()->update();
		sortTableKey();
		ui->m_table->sortByColumn(0, Qt::DescendingOrder);
		ui->m_table->sortByColumn(0, Qt::AscendingOrder);
}
void StationItem::keyChanged(Key& key, bool checkedPretone)
{
	Q_UNUSED(checkedPretone);
	auto indx = m_model.index(mh::selected(ui->m_table).row(), 3, {});
    LOG_ERROR(indx.isValid(), "Indx must be is Valid!");
//        if ((!m_defaultValues.value(mh::selected(ui->m_table).row()) && destinationIsEmpty))
//            key.setDestination(IPS_STR("None"));
//	qDebug() << QObject::tr("key.id:%1, max key:%2").arg(key.id()).arg(getMaxSupportKey(m_type));
	if (key.id() > (unsigned int)getMaxSupportKey(m_type)){
		m_settings.setKey(m_model.item(m_row));
		return;
	}
	m_model.setItem(mh::selected(ui->m_table).row(), key, {Qt::EditRole});
	sortTableKey();
	ui->m_table->sortByColumn(0, Qt::DescendingOrder);
	ui->m_table->sortByColumn(0, Qt::AscendingOrder);
	mh::simple::view::selectAndClickRow(ui->m_table, m_model, mh::selected(ui->m_table).row());
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
