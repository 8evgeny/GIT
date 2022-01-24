#include "include/view.h"
#include "classes/algo.h"
#include "stations_item.h"
#include "classes/defines.h"
#include "ui_stations_item.h"
#include "stations_item_instance.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/json.h"

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

StationsItem::StationsItem(BaseItem* item)
     : BaseItem(item)
{
       m_model.setParentStationItems(this);
}
StationsItem::~StationsItem()
{
    deshow();
}
void StationsItem::instanceConnectionToSettings()
{
//QMessageBox::StandardButton ret;
    QObject::connect(&m_settings, SIGNAL(stationChanged(const Station&)), this, SLOT(stationChanged(const Station&)));
    QObject::connect(&m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)));
}
void StationsItem::dataChanged(const QModelIndex& index, const QModelIndex&, QVector<int> const& roles)
{
	if(index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole){
        m_settings.setStation(m_model.station(index.row()));
		sortTableStations();
	}
}
void StationsItem::stationChanged(Station const& station)
{
        App::model()->tree()->update();
        if(mh::selected(ui->m_table).row() >= 0) {           
           if (station.cid() != 0 && !CidHelper::contains(station.cid())) {
               auto stationOfmodel = m_model.items().value(m_currentRow);
               LOG_FATAL(stationOfmodel, "st can not be null!");
               auto cid = stationOfmodel->cid();
				auto st_cid = station.cid();
               auto st = station;
               auto ip = stationOfmodel->ip();
               st.setIp(ip);
			   int currentMaxkey = getMaxSupportKey(stationOfmodel->type());
			   int newMaxKey =  getMaxSupportKey(station.type());
			   if (newMaxKey < currentMaxkey){
					QMessageBox::StandardButton ret = QMessageBox::question( this,  QApplication::applicationName(), QObject::tr("buttons with numbers over %1 will be deleted\nAccept new type station? Yes/No").arg(newMaxKey),
										QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
					if (ret == QMessageBox::Yes){
						m_model.items().at(m_currentRow)->DeleteKeys(newMaxKey+1, currentMaxkey);
					} else {
						return;
					}
			   }
               m_model.updateItem(mh::selected(ui->m_table).row(), st, {ModelStations::SettingsRole});
			   if (cid != st_cid){
					sortTableStations();
			   }
           }
           else {                 
                 auto stationOfmodel = m_model.items().value(m_currentRow);                 
                 LOG_FATAL(stationOfmodel, "st can not be null!");
                 auto st = station;
                 if (station.cid() == 0 || CidHelper::contains(station.cid()) || IpHelper::contains(station.ip())) {                     
                     auto cid = stationOfmodel->cid();
					 auto st_cid = station.cid();
                     QString ip;
                     if (IpHelper::contains(station.ip()))
                        ip = stationOfmodel->ip();                     
                     else
                        ip = station.ip();
                     st.setCid(cid);
                     st.setIp(ip);
					int currentMaxkey = getMaxSupportKey(stationOfmodel->type());
					int newMaxKey =  getMaxSupportKey(station.type());
					if (newMaxKey < currentMaxkey){
							QMessageBox::StandardButton ret = QMessageBox::question( this,  QApplication::applicationName(), QObject::tr("buttons with numbers over %1 will be deleted\nAccept new type station? Yes/No").arg(newMaxKey),
												QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
							if (ret == QMessageBox::Yes){
								m_model.items().at(m_currentRow)->DeleteKeys(newMaxKey+1, currentMaxkey);
							} else {
								return;
							}
					 }
                    m_model.updateItem(mh::selected(ui->m_table).row(), st, {ModelStations::SettingsRole});
	 			    if (cid != st_cid){
						sortTableStations();
					}
                 }
           }
       }
}
void StationsItem::sortTableStations()
{
	QVariantMap map;
	map.insert(IPS_STR("Stations"), m_model.sort_serialize());
	QJsonObject cnfObj(QJsonObject::fromVariantMap(map));
	clear();
	deserialize(cnfObj);
	ui->m_table->sortByColumn(0, Qt::DescendingOrder);
	ui->m_table->sortByColumn(0, Qt::AscendingOrder);
}

void StationsItem::fillTypes(QMenu* menu, QMenu* sub)
{
    sub->setIcon(QIcon(IPS_STR(":/svg/plus.svg")));
    for (auto&& type: station_type::defaultStationTypes()) {
        sub->addAction(toString(type), [this, type] () {
        auto item = StationItem::defValue(m_model.parentStationItems(), type, CidHelper::next(), CidHelper::next()
              , IpHelper::next(), APP_SETTINGS->netmask(), APP_SETTINGS->gateway(), QString::fromLocal8Bit(""));
            m_model.insertItem(m_model.rowCount(), item);
            ui->m_rem->setEnabled(true);
            App::model()->settings()->setVisible(true);
            mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.rowCount()-1);
        });
    }
    LOG_WARNING(!sub->isEmpty(), "Sub is empty");
    menu->addMenu(sub);
}
void StationsItem::createMenu()
{
    QMenu menu;
    QMenu sub(iMenu::tr("Add station"), &menu);
    fillTypes(&menu, &sub);
    auto rem = menu.addAction(QIcon(IPS_STR(":/svg/minus.svg")), iMenu::tr("Remove station"), [this]() {
        ui->m_rem->clicked();
    });
    menu.addSeparator();
    auto remAll = menu.addAction(QIcon(IPS_STR(":/svg/remove_all.svg")), iMenu::tr("Remove all stations"), [this]() {
        m_model.removeRows(0, m_model.rowCount());
        ui->m_rem->setEnabled(false);
        App::model()->settings()->setVisible(false);
    });
    remAll->setEnabled(m_model.rowCount());
    rem->setEnabled(m_model.rowCount());
    menu.exec(QCursor::pos());
}
void StationsItem::selectedRowDelete() noexcept
{
    auto const& indx = mh::selected(ui->m_table);
    if (!indx.isValid())
        return;

    m_model.removeRow(indx.row());
    mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()));

    if (m_currentRow >= m_model.rowCount())
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()-1));
}
void StationsItem::stationsSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    auto const& indx = mh::selected(ui->m_table);
    mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()));

    if (m_currentRow >= m_model.rowCount())
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, (indx.row() == 0 ? 0 : indx.row()-1));
}
void StationsItem::installSignals()
{
    connect(ui->m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(stationsSelectionChanged(const QItemSelection&,const QItemSelection&)));
    QObject::connect(ui->m_table, &QTableView::customContextMenuRequested, this, [this] (const QPoint&) {
         createMenu();
    });
//===вставка новой станции=====
    QObject::connect(ui->m_add, &QToolButton::clicked, this, [this]() {    
//вставка в модель новой станции
		if (m_model.insertRow(m_currentRow))
//разрешение действи€ кнопки "удалить станцию"
			ui->m_rem->setEnabled(true);
//разрешение видимости панели настройки
        App::model()->settings()->setVisible(true);
//эмул€ци€ выбора введенной станции
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.rowCount()-1);
		ui->m_table->sortByColumn(0, Qt::AscendingOrder);
    });
//===========================
    QObject::connect(ui->m_table, &QTableView::clicked, this, [this] (const QModelIndex& index) {
        m_index = index;
        m_currentRow = index.row();
        m_settings.setStation(m_model.station(index.row()));
        mh::simple::view::selectRow(ui->m_table, index.row());
    });
//====удаление выделенных станций
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
    selectedRowDelete();        
    if (!m_model.rowCount()) {
        ui->m_rem->setEnabled(false);        
        App::model()->settings()->setVisible(false);
    }
    });
//===============================
}
void StationsItem::on_sectionClicked ( int logicalIndex,  Qt::SortOrder order )
{
	ui->m_table->sortByColumn(logicalIndex, order);
}

void StationsItem::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (ui) {
            ui->retranslateUi(this);
            m_settings.retranslateUi();
        }
    }
    QWidget::changeEvent(event);
}
StationsItem* StationsItem::show(QWidget* content, QWidget* settings) {
    ips::algo::createContent(ui, content);
    ips::algo::removeChildren(settings);

    m_settings.setupUi(settings);
	m_model_proxy.setSourceModel(&m_model);
//    ui->m_table->setModel(&m_model);
	ui->m_table->setModel(&m_model_proxy);
	ui->m_table->setSortingEnabled(true);
//===сортировка по колонке===
	QHeaderView* m_horiz_header = ui->m_table->horizontalHeader();
	connect(m_horiz_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),  this, SLOT(on_sectionClicked(int, Qt::SortOrder)));
	ui->m_table->sortByColumn(0, Qt::AscendingOrder);

	ui->m_table->setShowGrid(false);
    ui->m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    StationsItemInstance(ui);

    ui->m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    installSignals();
    instanceConnectionToSettings();

    mh::simple::view::selectAndClickRow(ui->m_table, m_model, 0);
    App::model()->settings()->setVisible(m_model.rowCount());
    ui->m_rem->setEnabled(m_model.rowCount());
    return this;
}
void StationsItem::deshow()
{
//	QObject::tr("StationsItem::deshow()");
    QObject::disconnect(&m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)), this, SLOT(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int> const&)));
	QObject::disconnect(&m_settings, SIGNAL(stationChanged(const Station&)), this, SLOT(stationChanged(const Station&)));
    delete ui;
    ui = nullptr;
}
bool StationsItem::error(BaseItem*) const
{
    return true;
}
QString StationsItem::name(BaseItem*) const
{
    return iDesc::tr("Stations");
}
QJsonValue StationsItem::serialize() const
{
    return m_model.serialize();
}
template<typename T>
void StationsItem::clearList(QList<T>& list) noexcept
{
    while (list.size())
            list.removeAt(0);

    list.clear();
}
bool StationsItem::clear()
{    
//	qDebug() << QObject::tr("StationsItem::clear()");
    if (m_model.rowCount())
        m_model.removeRows(0, App::model()->stationsItem()->childCount());

    return true;
}
bool StationsItem::deserialize(QJsonObject const& data)
{    
//	qDebug() << QObject::tr("StationsItem::deserialize()");
    IPS_JSON_EXTRACT_ARRAY(data, s, IPS_STR("Stations"))
	return m_model.deserialize(s.toArray());
}
bool StationsItem::deserializeTracks(QJsonObject const& obj) noexcept
{
    auto tracks = obj.value(IPS_STR("saved_tracks"));
    if (tracks.isUndefined() || !tracks.isArray())
        return LOG_ERROR(false, "tracks is not found or invalid");
    else {
         auto arr = tracks.toArray();
         m_countTracks = m_countTracks + arr.size();
        return true;
    }
}

int StationsItem::getMaxSupportKey(StationType type)
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
