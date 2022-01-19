#include "ui_sip_settings.h"
#include "sip_settings.h"
#include "classes/defines.h"
#include "classes/json.h"
#include "classes/app.h"
#include "classes/cids_container.h"
#include <QCryptographicHash>
#include "classes/base_model/items/global_parameters_item/cid_helper.h"
#include "classes/base_model/items/global_parameters_item/global_parameters.h"

SipSettings::SipSettings()
    : ui(new Ui::SipSettings)
{    
}
SipSettings::~SipSettings()
{
    delete ui;
}
void SipSettings::setupUi(QWidget* parent)
{
    ui->setupUi(parent);
    sipStationItemsSettingsInstance(ui);    
    instanceSignals();    
    updateUi();
    if (!m_isCshow) {
        createCid();
        m_isCshow = true;
    }
}
void SipSettings::visibleCid() noexcept
{
    ui->m_cid->setText(m_cid);
}
void SipSettings::createCid() noexcept
{
    QByteArray arr;
    auto cid = CidHelper::next();
    arr.append(static_cast<char>(cid));
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(arr);
    QString hashStr;
    QLatin1String hashLatin(hash.result());
    hashStr.append(hashLatin);
    m_hash = hashStr;
    auto& cids = App::model()->globalParametersItem()->cids();
    cids.append(new CidsContainer(hashStr, cid));
    m_cid = QString::number(static_cast<int>(cid));
    ui->m_cid->setText(m_cid);
}
void SipSettings::instanceSignals() noexcept
{
    QObject::connect(ui->c_activate, SIGNAL(clicked(bool)), this, SIGNAL(checkBoxPosition(bool)));
    QObject::connect(ui->m_prefix, SIGNAL(textEdited(QString const&)), this, SIGNAL(prefixChanged(QString const&)));
    QObject::connect(ui->m_cid, SIGNAL(textEdited(QString const&)), this, SIGNAL(cidChanged(QString const&)));
    QObject::connect(ui->m_prefix, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_cid, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_login, SIGNAL(editingFinished()), this, SLOT(editData()));
    QObject::connect(ui->m_password, SIGNAL(editingFinished()), this, SLOT(editData()));
}
void SipSettings::editData() noexcept
{
    QRegularExpression reg;
    reg.setPattern(IPS_STR("^[0]"));
    if (isCid(ui->m_cid->text()) && !CidHelper::contains(toCid(ui->m_cid->text())) && !reg.match(ui->m_cid->text()).hasMatch()) {
        updateCid();
        m_cid = ui->m_cid->text();
    }
    m_prefix = ui->m_prefix->text();    
    m_login = ui->m_login->text();
    m_password = ui->m_password->text();
}
void SipSettings::updateUi() noexcept
{
    ui->m_prefix->setText(m_prefix);
    ui->m_cid->setText(m_cid);
    ui->m_login->setText(m_login);
    ui->m_password->setText(m_password);
}
QJsonValue SipSettings::serialize() const
{
    return QJsonObject {
            {IPS_STR("prefix"), m_prefix}
           ,{IPS_STR("cid"), m_cid}
           ,{IPS_STR("login"), m_login}
           ,{IPS_STR("password"), m_password}};
}
bool SipSettings::deserialize(QJsonObject const& data)
{
   IPS_JSON_TO_STR(data, IPS_STR("prefix"), prefix)
   m_prefix = prefix;

   IPS_JSON_TO_STR(data, IPS_STR("cid"), cid)
   m_cid = cid;

   IPS_JSON_TO_STR(data, IPS_STR("login"), login)
   m_login = login;

   IPS_JSON_TO_STR(data, IPS_STR("password"), password)
   m_password = password;
   return true;
}
void SipSettings::clear() noexcept
{
    ui->m_prefix->clear();
    ui->m_cid->clear();
    ui->m_login->clear();
    ui->m_password->clear();
    m_prefix.clear();
    m_login.clear();
    m_password.clear();
}
void SipSettings::setHash(QString const& hash) noexcept
{
    m_hash = hash;
}
QString const& SipSettings::toHash() noexcept
{
    return m_hash;
}
void SipSettings::updateCid() noexcept
{
    QString hash = toHash();
    auto& cids = App::model()->globalParametersItem()->cids();
    auto cid = m_cid;
    auto pos = CidsContainer::findPosCid(cids, hash);
    if (pos < cids.size()) {
        auto contCid = cids.value(pos);
        LOG_FATAL(contCid, "contCid can not be nulptr");
        contCid->setCid(cid.toUInt());
    }
}
