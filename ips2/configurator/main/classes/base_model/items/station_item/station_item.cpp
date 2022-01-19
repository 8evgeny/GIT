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

StationItem::StationItem(BaseItem* item)
    : inherited(item)
{
    m_model.setParentStationItem(this);
    m_isChecked = false;    
    m_settings.setModelTones(m_modelTones);
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
StationItem::class_name* StationItem::defValue(BaseItem* parent, StationType type, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway)
{
    auto result = new StationItem(parent);
    result->setIp(ip);    
    result->setId(id);
    result->setType(type);
    result->setStationMask(mask);
    result->setStationName(GT_STR("NewStation").arg(cid));
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
    return result;
}
StationItem::class_name* StationItem::defValue(BaseItem* parent, uint cid, uint id, QString const& ip, QString const& mask, QString const& gateway)
{
    return defValue(parent, StationType::UPW4, cid, id, ip, mask, gateway);
}
void StationItem::instanceConnectionToSettings()
{
    QObject::connect(&m_settings, &StationItemSettings::changedCid, [this] (cid_t cidStation) {
        if (cidStation != static_cast<uint>(cid()))
            m_settings.setCidStatus(true);
    });
    QObject::connect(&m_settings, &StationItemSettings::stationChanged, [this] (const Station& station) {
        App::model()->tree()->update();
        setStationName(station.stationName());
        if (station.cid() != 0 && !CidHelper::contains(station.cid()))
            setCid(station.cid());
        setType(station.type());
        if (!IpHelper::contains(station.ip()))
            setIp(station.ip());
        setStationMask(station.stationMask());
        setGateway(station.gateway());
    });
    QObject::connect(&m_settings, &StationItemSettings::keyChanged, [this] (Key& key, bool /*destinationIsEmpty*/) {
        auto indx = m_model.index(mh::selected(ui->m_table).row(), 3, {});
        LOG_ERROR(indx.isValid(), "Indx must be is Valid!");

//        if ((!m_defaultValues.value(mh::selected(ui->m_table).row()) && destinationIsEmpty))
//            key.setDestination(IPS_STR("None"));

        m_model.setItem(mh::selected(ui->m_table).row(), key, {Qt::EditRole});
    });
    QObject::connect(&m_model, &ModelKeys::dataChanged, [this] (const auto& index, const auto&, auto const& roles) {
        if (index.isValid() && roles.size() == 1 && roles[0] == Qt::EditRole)
            m_settings.setKey(m_model.item(index.row()));
    });    
}
void StationItem::selectedRowDelete() noexcept
{
    auto const& indx = mh::selected(ui->m_table);
    if (!indx.isValid())
        return;

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
    auto groupsItem = App::model()->groupItem();
    LOG_FATAL(groupsItem, "groupsItem can not be nullptr");
    QObject::connect(groupsItem, SIGNAL(updateGroupsList(itemsGroups_t const&)), this, SLOT(updaterGroupsList(itemsGroups_t const&)));
    QObject::connect(ui->m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(keysSelectionChanged(const QItemSelection&,const QItemSelection&)));
    QObject::connect(ui->m_table, &QTableView::customContextMenuRequested, this, [this](const QPoint &) {
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
    });
    QObject::connect(ui->m_table, &QTableView::clicked, [this] (const QModelIndex& index) {
        if (!index.isValid())
            return;

        m_row = index.row();        
        auto key = m_model.item(m_row);        
        m_settings.setKey(key);                
        mh::simple::view::selectRow(ui->m_table, index.row());
        m_settings.hideSettings(true);
        m_settings.setSettingsPretone();
    });
    QObject::connect(ui->m_table, &QTableView::doubleClicked, [this] (const QModelIndex& index) {
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
            TypeListDialog<QStringList> dlg(iFunction::tr("Destiations"), cidGtList(), key.destination());
            if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {               
                 key.setDestination(dlg.current());
                m_model.setItem(mh::selected(ui->m_table).row(), key, {Qt::EditRole});
            }
        }
    });
    QObject::connect(ui->m_add, &QToolButton::clicked, this, [this]() {        
        m_model.insertRows(m_model.rowCount(), 1);
        ui->m_rem->setEnabled(true);
        m_settings.setKeySettingsEnabled(true, m_model.rowCount());
        mh::simple::view::selectAndClickRow(ui->m_table, m_model, m_model.items().size()-1);
        auto const& indx = mh::selected(ui->m_table);
        m_defaultValues.insert(indx.row(), false);
        m_settings.hideSettings(true);
        if (m_model.rowCount() == MAX_COUNT_KEYS) {
            ui->m_add->setEnabled(false);
            LimitRowsDialog dlg;
            dlg.exec();
        }
    });
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
        ui->m_add->setEnabled(true);
        auto const& indx = mh::selected(ui->m_table);
        if (!indx.isValid())
            return;

        m_defaultValues[indx.row()] = true;
        selectedRowDelete();

        if (!m_model.rowCount())
           m_settings.hideSettings(false);

        if (!m_model.rowCount()) {
           ui->m_rem->setEnabled(false);           
           m_settings.setKeySettingsEnabled(false, m_model.rowCount());
        }
    });    
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
    ui->m_table->setShowGrid(false);
    ui->m_table->setContextMenuPolicy(Qt::CustomContextMenu);

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
