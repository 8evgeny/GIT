#include "editor_button.h"
#include "classes/json.h"
#include "station_item_settings.h"
#include "classes/combobox_algo.h"
#include "ui_station_item_settings.h"
#include "station_item_settings_instance.h"
#include "include/validators/value_int_validator.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/station_item/model_keys/key_helper.h"
#include "dialogs/stations/activate_control_lines/activate_control_lines.h"
#include <QStyledItemDelegate>

StationItemSettings::StationItemSettings()
    : ui(new Ui::StationItemSettings)
{        
}
StationItemSettings::~StationItemSettings()
{
    delete ui;
}
void StationItemSettings::initPretones() noexcept
{
    m_edit = new EditorButton(ui);
    m_edit->setText(m_pretone);
    m_lay = new QHBoxLayout(ui->m_key_pretone_play);
    m_lay->setMargin(0);
    m_lay->addWidget(m_edit);
}
void StationItemSettings::setupUi(QWidget* parent, Station* station, StationItem* StationItem)
{    
    m_parent = parent;
    m_station = StationItem;
    m_pretone = IPS_STR("");
    ui->setupUi(parent);
    StationItemSettingsInstance(ui);
    initPretones();
    ui->c_pretone->setChecked(false);   
    ui->m_key_pretone_play->setEnabled(false);
    ui->m_key_pretone_duration->setEnabled(false);
    ui->m_key_pretone_times->setValidator(new QIntValidator(0, 10000));
    setStation(station);
    instanceKeySignals();
    instanceStationSignals();

    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->m_station_type->setItemDelegate(itemDelegate);
    ui->m_key_func->setItemDelegate(itemDelegate);
    ui->m_key_dest->setItemDelegate(itemDelegate);
    ui->m_key_pretone_duration->setItemDelegate(itemDelegate);
}
void StationItemSettings::retranslateUi()
{
    ui->retranslateUi(m_parent);
}
void StationItemSettings::hideSettings(bool is) noexcept
{   
    ui->m_key_activate_lines->setVisible(is);
    ui->l_key_activate_lines->setVisible(is);
    ui->l_key_id->setVisible(is);
    ui->m_key_id->setVisible(is);
    ui->m_key_dest->setVisible(is);
    ui->l_key_dest->setVisible(is);
    ui->m_key_func->setVisible(is);
    ui->l_key_func->setVisible(is);
    ui->m_key_name->setVisible(is);
    ui->l_key_name->setVisible(is);
    ui->b_key_activate_lines->setVisible(is);
    ui->l_additional_caption->setVisible(is);
    ui->l_key_priority->setVisible(is);
    ui->m_key_priority->setVisible(is);
    ui->l_key_activate_lines->setVisible(is);
    ui->l_key_caption->setVisible(is);
    ui->l_key_id->setVisible(is);
    ui->c_pretone->setVisible(is);
    ui->l_key_pretone_play->setVisible(is);
    ui->l_key_pretone_duration->setVisible(is);
    ui->m_key_pretone_play->setVisible(is);
    ui->m_key_pretone_duration->setVisible(is);

    ui->m_key_pretone_times->setHidden(!ui->c_pretone->isChecked());
    ui->l_key_pretone_times->setHidden(!ui->c_pretone->isChecked());
}
void StationItemSettings::setSettingsPretone() noexcept
{
    ui->m_key_pretone_times->show();
    ui->l_key_pretone_times->show();
    if ((m_savePretone == DurationType::ONCE) || (m_savePretone == DurationType::INFINITE)) {
        ui->m_key_pretone_times->setHidden(true);
        ui->l_key_pretone_times->setHidden(true);
    }
}
void StationItemSettings::editingIP(QLineEdit* ipType, QString& value) noexcept
{
    int pos = ipType->text().length();
    auto text = ipType->text();
    IpValidator ip(ipType);
    if (ip.validate(text, pos) != QValidator::Acceptable) {
        ipType->setText(value);
    }
    value = ipType->text();
}
void StationItemSettings::instanceStationSignals() noexcept
{
    QObject::connect(ui->m_station_name, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
	QObject::connect(ui->m_station_SN, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_mask, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_gateway, SIGNAL(editingFinished()), this, SLOT(dataStationChanged()));
    QObject::connect(ui->m_station_type, SIGNAL(currentIndexChanged(int)), this, SLOT(dataStationChanged()));    
    QObject::connect(ui->m_station_cid, &QLineEdit::editingFinished, this, [this] () {
    auto cid = toCid(ui->m_station_cid->text());
    if (CidHelper::contains(cid) && m_cidStatus) {
          ui->m_station_cid->setText(convert::toString(m_cid));
          m_cidStatus = false;
    }
     else
         dataStationChanged();
    });
    QObject::connect(ui->m_station_ip, &QLineEdit::editingFinished, this, [this] () {dataStationChanged();});
}
void StationItemSettings::instanceKeySignals() noexcept
{        
    QObject::connect(ui->m_key_pretone_duration, &QComboBox::currentTextChanged, this, [this] (const QString& text) {
        if (text == IPS_STR("Invalid"))
            ui->m_key_pretone_duration->setCurrentText(m_duration);
    });
    QObject::connect(m_edit, SIGNAL(textChanged(QString const&)), this, SLOT(pretoneChanged(QString const&)));
    QObject::connect(ui->m_key_id, &QLineEdit::editingFinished, this, [this] () {
        if (!KeyHelper::contains(static_cast<uint>(ui->m_key_id->text().toInt()), m_station))
           dataKeyChanged();
        else ui->m_key_id->setText(QString::number(static_cast<int>(m_id)));
    });
    QObject::connect(ui->m_key_pretone_times, SIGNAL(textChanged(QString)), this, SLOT(dataKeyChanged()));
    QObject::connect(ui->m_key_name, SIGNAL(textChanged(QString)), this, SLOT(dataKeyChanged()));
    QObject::connect(ui->m_key_priority, SIGNAL(textChanged(QString)), this, SLOT(dataKeyChanged()));
    QObject::connect(ui->m_key_activate_lines, SIGNAL(textChanged(QString)), this, SLOT(dataKeyChanged()));    
    QObject::connect(ui->m_key_pretone_play, SIGNAL(textChanged(QString const&)), this, SLOT(pretonePlayChanged(QString const&)));
    QObject::connect(ui->c_pretone, SIGNAL(clicked(bool)), this, SLOT(dataKeyChangedFlag(bool)));
    QObject::connect(ui->m_key_pretone_duration, SIGNAL(currentTextChanged(QString)), this, SLOT(dataKeyChanged(QString const&)));
    QObject::connect(ui->m_key_func, SIGNAL(currentIndexChanged(QString const&)), this, SLOT(functionChanged()));
    QObject::connect(ui->b_key_activate_lines, &QPushButton::clicked, [this] () {
        ActivateControlLinesDialog dlg(cidGtList());        
        if(dlg.exec() == ActivateControlLinesDialog::Accepted) {
            auto const& val = ui->m_key_activate_lines->text();
            ui->m_key_activate_lines->setText(IPS_STR("%1%2.%3;").arg(val, dlg.cid(), dlg.value()));
        }
    });
    QObject::connect(ui->c_pretone, &QCheckBox::clicked, this, [this] (bool checked) {
        ui->m_key_pretone_play->setEnabled(checked);
        ui->m_key_pretone_duration->setEnabled(checked);
        if ((durationFromString(ui->m_key_pretone_duration->currentText()) == DurationType::seconds) || (durationFromString(ui->m_key_pretone_duration->currentText()) == DurationType::times)) {
            ui->m_key_pretone_times->setHidden(!checked);
            ui->l_key_pretone_times->setHidden(!checked);
        }
    });
    QObject::connect(ui->m_key_dest, &QComboBox::currentTextChanged, [this] (QString const& dest) {
        if (m_func_call)
            sendKey(convert::toUint(ui->m_key_priority->text()), dest);
    });
}
void StationItemSettings::pretoneChanged(const QString& pretone) noexcept
{
    m_pretone = pretone;   
    m_edit = new EditorButton(ui);
    m_edit->setText(m_pretone);
    ui->m_key_pretone_play->textChanged(m_pretone);
    sendKey(Key::priorityFromFucntionType(gt::cb::current<FunctionType>(ui->m_key_func)), {}, false);
}
void StationItemSettings::setKeySettingsEnabled(bool is, int countKeys) noexcept
{    
    ui->c_pretone->setEnabled(is);    
    ui->m_key_id->setEnabled(is);
    ui->m_key_name->setEnabled(is);
    ui->m_key_priority->setEnabled(is);
    ui->m_key_func->setEnabled(is);
    ui->m_key_dest->setEnabled(is);
    ui->m_key_activate_lines->setValidator(new ActivateControlValidator);
    ui->m_key_activate_lines->setEnabled(is);
    ui->b_key_activate_lines->setEnabled(is);

    if (!countKeys) {
        ui->c_pretone->setChecked(false);
        ui->m_key_pretone_play->setEnabled(false);
        ui->m_key_pretone_duration->setEnabled(false);
    }
}
void StationItemSettings::setKey(Key const& key) noexcept
{    
    m_func_call = false;
    ui->m_key_id->setMaxLength(256);    
    ui->m_key_id->setValidator(new QIntValidator(1, 999, ui->m_key_id));
    ui->m_key_id->setText(convert::toString(key.id()));
    m_id = key.id();
    ui->m_key_name->setMaxLength(64);
    ui->m_key_name->setText(key.name());    
    ui->m_key_dest->setCurrentText(key.destination());
    ui->m_key_priority->setMaxLength(2);
    ui->m_key_priority->setText(convert::toString(key.priority()));
    gt::cb::setCurrent(ui->m_key_func, key.function());
    ui->m_key_pretone_play->setMaxLength(256);

    if (key.pretone().id()) {
       m_pretone = convert::toString(key.pretone().id());
       m_edit->setText(m_pretone);
       ui->m_key_pretone_play->textChanged(m_pretone);       
    }
    else {
          m_pretone = IPS_STR("");
          m_edit->setText(m_pretone);
          emit(textChanged(m_pretone));          
          ui->m_key_pretone_play->textChanged(m_pretone);
    }
    m_savePretone = key.pretone().duration();        
    ui->m_key_pretone_times->setText(QString::number(key.times()));
    ui->m_key_pretone_duration->setCurrentText(durationToString(key.pretone().duration()));
    ui->m_key_activate_lines->setMaxLength(256);
    ui->m_key_activate_lines->setText(key.activateControlLines());    
    ui->c_pretone->setChecked(key.pretoneChecked());
    ui->m_key_pretone_play->setEnabled(key.pretoneChecked());
    ui->m_key_pretone_duration->setEnabled(key.pretoneChecked());    
    ui->c_pretone->setChecked(key.pretoneChecked());
    m_func_call = true;
    m_duration = ui->m_key_pretone_duration->currentText();
}
void StationItemSettings::setType(StationType const& type) noexcept
{
    QObject::disconnect(ui->m_station_type, SIGNAL(currentIndexChanged(int)), this, SLOT(dataStationChanged()));    
	gt::cb::setCurrent(ui->m_station_type, type);
    QObject::connect(ui->m_station_type, SIGNAL(currentIndexChanged(int)), this, SLOT(dataStationChanged()));    
}
void StationItemSettings::setStation(Station* station) noexcept
{    
    m_ip = station->ip();
	m_sn = station->sn();
    m_mask = station->stationMask();
    m_gateway = station->gateway();
    m_cid = station->cid();
    editingIP(ui->m_station_ip, m_ip);
    editingIP(ui->m_station_mask, m_mask);
    editingIP(ui->m_station_gateway, m_gateway);
    ui->m_station_name->setMaxLength(64);
    ui->m_station_name->setText(station->stationName());    
    ui->m_station_cid->setValidator(new QIntValidator(1, MAX_CID_VALUE, ui->m_station_cid));
    ui->m_station_cid->setText(convert::toString(station->cid()));
	ui->m_station_SN->setText(m_sn);
	m_type = station->type();
    gt::cb::setCurrent(ui->m_station_type, station->type());
}
void StationItemSettings::dataStationChanged()
{   
    Station station;
    editingIP(ui->m_station_ip, m_ip);
    editingIP(ui->m_station_mask, m_mask);
    editingIP(ui->m_station_gateway, m_gateway);
    station.setStationName(ui->m_station_name->text());    
    station.setIp(ui->m_station_ip->text());
	station.setSN(ui->m_station_SN->text());
    station.setStationMask(ui->m_station_mask->text());    
    station.setCid(convert::toUint(ui->m_station_cid->text()));
    station.setType(gt::cb::current<StationType>(ui->m_station_type));
    station.setGateway(ui->m_station_gateway->text());
    emit(stationChanged(station));
    emit(changedCid(convert::toUint(ui->m_station_cid->text())));
}
void StationItemSettings::pretonePlayChanged(QString const&) noexcept
{
    if (m_func_call)
        sendKey(Key::priorityFromFucntionType(gt::cb::current<FunctionType>(ui->m_key_func)), {}, false);    
}
void StationItemSettings::dataKeyChangedFlag(bool)
{
    if (m_func_call)
        sendKey(convert::toUint(ui->m_key_priority->text()), ui->m_key_pretone_duration->currentText());
}
void StationItemSettings::dataKeyChanged(QString const& destination)
{    
    if (m_func_call)
        sendKey(convert::toUint(ui->m_key_priority->text()), destination);
}
void StationItemSettings::functionChanged()
{    
    if (m_func_call)
        sendKey(Key::priorityFromFucntionType(gt::cb::current<FunctionType>(ui->m_key_func)));
}
void StationItemSettings::hidePretone() noexcept
{
    ui->m_key_pretone_times->show();
    ui->l_key_pretone_times->show();
    if (m_savePretone == DurationType::seconds)
        ui->l_key_pretone_times->setText(IPS_STR("Seconds"));
    if (m_savePretone == DurationType::times)
        ui->l_key_pretone_times->setText(IPS_STR("Times"));

    if ((m_savePretone == DurationType::ONCE) || (m_savePretone == DurationType::INFINITE)) {
        ui->m_key_pretone_times->setHidden(true);
        ui->l_key_pretone_times->setHidden(true);
    }
}
void StationItemSettings::sendKey(uint priority, QString const& destination, bool pretoneChanged) noexcept
{        
    Key key;    
    auto dest = destination;
    auto id = convert::toUint(ui->m_key_id->text());
    if (id) key.setId(id);
    else key.setId(m_id);
    key.setName(ui->m_key_name->text());
    key.setPriority(priority);
    key.setFunction(gt::cb::current<FunctionType>(ui->m_key_func));
    key.setDestination(ui->m_key_dest->currentText());
    key.setActivateControlLines(ui->m_key_activate_lines->text());   
    key.setPretoneId(convert::toUint(m_pretone));    
    key.setTimes(ui->m_key_pretone_times->text().toInt());
    if (pretoneChanged && !destination.isEmpty()) {
        key.setPretoneDuration(durationFromString(destination));
        m_savePretone = durationFromString(destination);
        hidePretone();
        dest = QString();
    }
    else
        key.setPretoneDuration(m_savePretone);
    key.setPretoneChecked(ui->c_pretone->isChecked());    
    emit(keyChanged(key, dest.isEmpty()));        
}
