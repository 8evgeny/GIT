#include "include/view.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/defines.h"
#include "ui_sip_stations_item.h"
#include "sip_stations_item_instance.h"
#include "dialogs/type_list/type_list.h"
#include "classes/base_model/items/interfaces/sip_stations_item/sip_stations_item.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"

SipStationsItem::SipStationsItem(BaseItem* item)
     : BaseItem(item)
     , m_modelDest(new ModelDestination())
     , m_modelRules(new ModelRules())
     , m_settings(new SipSettings())
{
}
SipStationsItem::~SipStationsItem()
{
    delete m_modelDest;
    delete m_modelRules;
    delete m_settings;
    deshow();
}
void SipStationsItem::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (ui)
            ui->retranslateUi(this);        
    }
    QWidget::changeEvent(event);
}
SipStationsItem* SipStationsItem::show(QWidget* content, QWidget* settings) {
    ips::algo::createContent(ui, content);
    ips::algo::removeChildren(settings);    
    App::model()->settings()->setVisible(m_modelDest->rowCount());
    m_settings->setupUi(settings);
    sipStationsItemInstance(ui, m_modelDest, m_modelRules);
    mh::simple::view::selectAndClickRow(ui->m_table_destiations, *m_modelDest, m_modelDest->rowCount()-1);
    mh::simple::view::selectAndClickRow(ui->m_table_rules_routings, *m_modelRules, m_modelRules->rowCount()-1);
    installSignals();
    return this;
}
void SipStationsItem::deshow()
{
    delete ui;
    ui = nullptr;
}
bool SipStationsItem::error(BaseItem*) const
{
    return true;
}
QString SipStationsItem::name(BaseItem*) const
{
    return iDesc::tr("SIP");
}
QJsonValue SipStationsItem::serialize() const
{
    return QJsonObject {{IPS_STR("Settings"), m_settings->serialize()}
                      , {IPS_STR("Rules"), m_modelRules->serialize()}
                      , {IPS_STR("Destination"), m_modelDest->serialize()}
    };
}
bool SipStationsItem::clear()
{    
    ips::algo::clear(m_modelDest);
    ips::algo::clear(m_modelRules);
    return true;
}
bool SipStationsItem::deserialize(QJsonObject const& data)
{
    IPS_JSON_EXTRACT_OBJECT(data, settings, IPS_STR("Settings"))
    IPS_JSON_EXTRACT_ARRAY(data, arrDest, IPS_STR("Destination"))
    IPS_JSON_EXTRACT_ARRAY(data, arrRules, IPS_STR("Rules"))
    m_modelDest->deserialize(arrDest.toArray());
    m_modelRules->deserialize(arrRules.toArray());
    m_settings->deserialize(settings.toObject());
    return true;
}
inline auto SipStationsItem::destsList() noexcept {
    QStringList list;
    auto const& dests = m_modelDest->items();
    for (auto dest : dests)
        list.append(dest.name());

    return list;
}
void SipStationsItem::installSignals() noexcept
{
    QObject::connect(m_settings, &SipSettings::prefixChanged, [this] (QString const& prefix) {m_prefix = prefix;});
    QObject::connect(m_settings, &SipSettings::cidChanged, [this] (QString const& cid) {m_cid = cid.toUInt();});    
    ui->m_table_destiations->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->m_table_rules_routings->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->m_table_destiations, &QTableView::clicked, this, [this] (const QModelIndex& index) {
        m_index = index;
        m_currentRow = index.row();
        mh::simple::view::selectRow(ui->m_table_destiations, index.row());
    });
    QObject::connect(ui->m_table_rules_routings, &QTableView::clicked, this, [this] (const QModelIndex& index) {
       m_index = index;
       m_currentRow = index.row();
       auto rule = m_modelRules->items().value(index.row());
       if (index.column() == 1 && !destsList().isEmpty()) {
           TypeListDialog<QStringList> dlg(iFunction::tr("Destiations"), destsList(), rule.destination());
           if (dlg.exec() == TypeListDialog<QStringList>::Accepted) {
               rule.setDestination(dlg.current());
               m_modelRules->setItem(mh::selected(ui->m_table_rules_routings).row(), rule, {Qt::EditRole});
           }
       }
       mh::simple::view::selectRow(ui->m_table_rules_routings, index.row());
    });
    QObject::connect(ui->m_add, &QToolButton::clicked, this, [this]() {
        m_modelDest->insertRow(m_modelDest->rowCount());
        ui->m_rem->setEnabled(m_modelDest->rowCount());
        m_settings->visibleCid();
        App::model()->settings()->setVisible(true);
        mh::simple::view::selectAndClickRow(ui->m_table_destiations, *m_modelDest, m_modelDest->rowCount()-1);
    });
    QObject::connect(ui->m_rem, &QToolButton::clicked, [this] () {
    ips::algo::selectedRowDelete<ModelDestination, QTableView>(m_modelDest, ui->m_table_destiations, m_currentRow);
    if (!m_modelDest->rowCount()) {
        ui->m_rem->setDisabled(true);
        App::model()->settings()->setVisible(false);
        m_settings->clear();
        mh::simple::view::selectAndClickRow(ui->m_table_destiations, *m_modelDest, m_modelDest->rowCount()-1);
    }
    });
    QObject::connect(ui->m_add_rules, &QToolButton::clicked, this, [this]() {
        m_modelRules->insertRow(m_modelRules->rowCount());
        ui->m_rem_rules->setEnabled(m_modelRules->rowCount());
        mh::simple::view::selectAndClickRow(ui->m_table_rules_routings, *m_modelRules, m_modelRules->rowCount()-1);
    });
    QObject::connect(ui->m_rem_rules, &QToolButton::clicked, [this] () {
    ips::algo::selectedRowDelete<ModelRules, QTableView>(m_modelRules, ui->m_table_rules_routings, m_currentRow);
    if (!m_modelRules->rowCount()) {
        ui->m_rem_rules->setDisabled(true);
        mh::simple::view::selectAndClickRow(ui->m_table_rules_routings, *m_modelRules, m_modelRules->rowCount()-1);
    }
    });    
    ips::algo::contextMenu<ModelDestination, SipSettings, QTableView, QToolButton>(m_modelDest, m_settings, ui->m_table_destiations, ui->m_add, ui->m_rem, iMenu::tr("Add Destination"), iMenu::tr("Remove Destination"), iMenu::tr("Remove all Destinations"));
    ips::algo::contextMenu<ModelRules, SipSettings, QTableView, QToolButton>(m_modelRules, m_settings, ui->m_table_rules_routings, ui->m_add_rules, ui->m_rem_rules, iMenu::tr("Add Rule"), iMenu::tr("Remove Rule"), iMenu::tr("Remove all Rules"));
}
