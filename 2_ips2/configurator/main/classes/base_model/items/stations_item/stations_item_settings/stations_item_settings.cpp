#include "include/convert/string.h"
#include "stations_item_settings.h"
#include "ui_stations_item_settings.h"
#include "stations_item_settings_instance.h"
#include "classes/combobox_algo.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "include/validators/value_int_validator.h"
#include "classes/base_model/items/stations_item/stations_item.h"

#include <QStyledItemDelegate>

StationsItemSettings::StationsItemSettings()
    : ui(new Ui::StationsItemSettings)    
{    
}
StationsItemSettings::~StationsItemSettings()
{
    delete ui;
}
void StationsItemSettings::setupUi(QWidget* parent)
{
    m_parent = parent;
    ui->setupUi(parent);        
    StationItemsSettingsInstance(ui);
    instanceSignals();
}
void StationsItemSettings::retranslateUi()
{
    ui->retranslateUi(m_parent);
}
void StationsItemSettings::setStation(Station const& station) noexcept
{
    QObject::disconnect(ui->m_station_type, SIGNAL(currentIndexChanged(int)), this, SLOT(dataStationChanged()));
    m_ip = station.ip();
    m_mask = station.stationMask();
    m_gateway = station.gateway();
    m_cid = station.cid();

    ui->m_station_name->setMaxLength(64);    
    ui->m_station_name->setText(station.stationName());   
    ui->m_station_cid->setValidator(new QIntValidator(1, 9999, ui->m_station_cid));
    ui->m_station_cid->setMaxLength(256);   
    ui->m_station_cid->setText(convert::toString(station.cid()));
    gt::cb::setCurrent(ui->m_station_type, station.type());
    ui->m_station_ip->setText(station.ip());
    ui->m_station_mask->setValidator(new IpValidator(ui->m_station_mask));
    ui->m_station_mask->setText(station.stationMask());
    ui->m_station_gateway->setValidator(new IpValidator(ui->m_station_gateway));
    ui->m_station_gateway->setText(station.gateway());    
    QObject::connect(ui->m_station_type, SIGNAL(currentIndexChanged(int)), this, SLOT(dataStationChanged()));

    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->m_station_type->setItemDelegate(itemDelegate);
}
void StationsItemSettings::editingHost(QLineEdit* ipType, QString& value) noexcept
{
    int pos = ipType->text().length();
    auto text = ipType->text();
    IpValidator ip(ipType);
    if ((ip.validate(text, pos) != QValidator::Acceptable))
        ipType->setText(value);
    value = ipType->text();
}
void StationsItemSettings::editingIP(QLineEdit* ipType, QString& value) noexcept
{
    int pos = ipType->text().length();
    auto text = ipType->text();
    IpValidator ip(ipType);
    if ((ip.validate(text, pos) != QValidator::Acceptable) || IpHelper::contains(text))
        ipType->setText(value);
    value = ipType->text();
}
void StationsItemSettings::instanceSignals() noexcept
{
    QObject::connect(ui->m_station_name, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_cid, &QLineEdit::editingFinished, this, [this] () {
        auto cid = toCid(ui->m_station_cid->text());
        if (!CidHelper::contains(cid) && cid !=0)
            dataStationChanged();
    });
    QObject::connect(ui->m_station_mask, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_gateway, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_ip, &QLineEdit::editingFinished, this, [this] () {dataStationChanged();});
}
void StationsItemSettings::dataStationChanged()
{
    Station station;    
    editingIP(ui->m_station_ip, m_ip);
    editingHost(ui->m_station_mask, m_mask);
    editingHost(ui->m_station_gateway, m_gateway);
    station.setStationName(ui->m_station_name->text());
    station.setIp(ui->m_station_ip->text());
    station.setStationMask(ui->m_station_mask->text());
    station.setCid(convert::toUint(ui->m_station_cid->text()));
    station.setType(gt::cb::current<StationType>(ui->m_station_type));
    station.setGateway(ui->m_station_gateway->text());
    emit stationChanged(station);
}
