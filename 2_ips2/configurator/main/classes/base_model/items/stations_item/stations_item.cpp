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
    QObject::connect(&m_settings, &StationsItemSettings::stationChanged, [this] (const Station& station) {
        App::model()->tree()->update();
        if(mh::selected(ui->m_table).row() >= 0) {           
           if (station.cid() != 0 && !CidHelper::contains(station.cid())) {
               auto stationOfmodel = m_model.items().value(m_currentRow);
               LOG_FATAL(stationOfmodel, "st can not be null!");
               auto st = station;
               auto ip = stationOfmodel->ip();
               st.setIp(ip);
               m_model.updateItem(mh::selected(ui->m_table).row(), st, {ModelStations::SettingsRole});
           }
           else {                 
                 auto stationOfmodel = m_model.items().value(m_currentRow);                 
                 LOG_FATAL(stationOfmodel, "st can not be null!");
                 auto st = station;
                 if (station.cid() == 0 || CidHelper::contains(station.cid()) || IpHelper::contains(station.ip())) {                     
                     auto cid = stationOfmodel->cid();
                     QString ip;
                     if (IpHelper::contains(station.ip()))
                        ip = stationOfmodel->ip();                     
                     else
                        ip = station.ip();                     
                     st.setCid(cid);
                     st.setIp(ip);
                     m_model.updateItem(mh::selected(ui->m_table).row(), st, {ModelStations::SettingsRole});
                 }
           }
       }
    });
    QObject::connect(&m_model, &ModelStations::dataChanged, [this] (const auto& index, const auto&, auto const& roles) {
        if(index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole)
           m_settings.setStation(m_model.station(index.row()));
    });
}
void StationsItem::fillTypes(QMenu* menu, QMenu* sub)
{
    sub->setIcon(QIcon(IPS_STR(":/svg/plus.svg")));
    for (auto&& type: station_type::defaultStationTypes()) {
        sub->addAction(toString(type), [this, type] () {
        auto item = StationItem::defValue(m_model.parentStationItems(), type, CidHelper::next(), CidHelper::next()
              , IpHelper::next(), APP_SETTINGS->netmask(), APP_SETTINGS->gateway());
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
    QObject::connect(ui->m_add, &QToolButton::clicked, this, [this]() {    
        m_model.insertRow(m_model.rowCount({}));
        ui->m_rem->setEnabled(true);        
        App::model()->settings()->setVisible(true);
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.rowCount()-1);
    });
    QObject::connect(ui->m_table, &QTableView::clicked, this, [this] (const QModelIndex& index) {
        m_index = index;
        m_currentRow = index.row();
        m_settings.setStation(m_model.station(index.row()));
        mh::simple::view::selectRow(ui->m_table, index.row());
    });
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
    selectedRowDelete();        
    if (!m_model.rowCount()) {
        ui->m_rem->setEnabled(false);        
        App::model()->settings()->setVisible(false);
    }
    });
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
    ui->m_table->setModel(&m_model);
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
    if (m_model.rowCount())
        m_model.removeRows(0, App::model()->stationsItem()->childCount());

    return true;
}
bool StationsItem::deserialize(QJsonObject const& data)
{    
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
