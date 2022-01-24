#include "classes/json.h"
#include "classes/algo.h"
#include "classes/defines.h"
#include "global_parameters.h"
#include "include/convert/string.h"
#include "include/validators/ip_validator.h"
#include "ui_global_parameters.h"

void GlobalParametersItem::instanceDataGlobal()
{       
    ui->m_pa->setValidator(new QIntValidator(0, 10, ui->m_pa));
    ui->m_pa->setText(convert::toString(publicAddress()));
    ui->m_alarm->setValidator(new QIntValidator(0, 10, ui->m_alarm));
    ui->m_alarm->setText(convert::toString(alarm()));
    ui->m_dynamic_group_call->setValidator(new QIntValidator(0, 10, ui->m_dynamic_group_call));
    ui->m_dynamic_group_call->setText(convert::toString(dynamicGroupCall()));
    ui->m_group_pa->setValidator(new QIntValidator(0, 10, ui->m_group_pa));
    ui->m_group_pa->setText(convert::toString(groupPa()));
    ui->m_conference->setValidator(new QIntValidator(0, 10, ui->m_conference));
    ui->m_conference->setText(convert::toString(conference()));
    ui->m_duplex->setValidator(new QIntValidator(0, 10, ui->m_duplex));
    ui->m_duplex->setText(convert::toString(duplex()));
    ui->m_caller_id->setValidator(new QIntValidator(0, MAX_CID_VALUE, ui->m_caller_id));
    ui->m_caller_id->setText(convert::toString(callerid()));  
    ui->m_ip->setValidator(new IpValidator);
    ui->m_ip->setText(ip());
    ui->m_netmask->setValidator(new IpValidator);
    ui->m_netmask->setText(netmask());
    ui->m_gateway->setValidator(new IpValidator);
    ui->m_gateway->setText(gateway());    
}
void GlobalParametersItem::editingCID(QLineEdit* cidType, uint& value) noexcept
{
    auto text = cidType->text();
    if (text.toUInt() == 0)
       cidType->setText(QString::number(static_cast<int>(value)));
    value = cidType->text().toUInt();
}
void GlobalParametersItem::editingIP(QLineEdit* ipType, QString& value) noexcept
{
    int pos = ipType->text().length();
    auto text = ipType->text();
    IpValidator ip(ipType);
    if (ip.validate(text, pos) != QValidator::Acceptable)
       ipType->setText(value);
    value = ipType->text();
}
void GlobalParametersItem::instanceSignalsGlobal()
{
    QObject::connect(ui->m_pa, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_alarm, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_dynamic_group_call, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_conference, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_duplex, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_group_pa, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);    
    QObject::connect(ui->m_caller_id, &QLineEdit::editingFinished, this, [this] {editingCID(ui->m_caller_id, m_callerid);});
    QObject::connect(ui->m_ip, &QLineEdit::editingFinished, this, &GlobalParametersItem::dataChanged);
    QObject::connect(ui->m_netmask, &QLineEdit::editingFinished, this, [this] {editingIP(ui->m_netmask, m_netmask);});
    QObject::connect(ui->m_gateway, &QLineEdit::editingFinished, this, [this] {editingIP(ui->m_gateway, m_gateway);});
    QObject::connect(ui->m_ip, &QLineEdit::editingFinished, this, [this] {editingIP(ui->m_ip, m_ip);});
}
void GlobalParametersItem::changeEvent(QEvent* event)
{
    IPS_LANGUGE_EVENT(ui, this)
    QWidget::changeEvent(event);
}
void GlobalParametersItem::dataChanged()
{
    m_pa = convert::toUint(ui->m_pa->text());
    m_duplex = convert::toUint(ui->m_duplex->text());
    m_alarm = convert::toUint(ui->m_alarm->text());
    m_group_pa = convert::toUint(ui->m_group_pa->text());
    m_conference = convert::toUint(ui->m_conference->text());
    m_callerid = convert::toUint(ui->m_caller_id->text());
    m_dynamic_group_call = convert::toUint(ui->m_dynamic_group_call->text());    
}
GlobalParametersItem::~GlobalParametersItem()
{
    deshow();
}
GlobalParametersItem* GlobalParametersItem::show(QWidget* content, QWidget* settings)
{
    ips::algo::removeChildren(content);
    ips::algo::removeChildren(settings);
    ips::algo::createContent(ui, content);
    instanceDataGlobal();
    instanceSignalsGlobal();
    return this;
}
void GlobalParametersItem::deshow()
{
    delete ui;
    ui = nullptr;
}
QString GlobalParametersItem::name(BaseItem*) const
{
    return iInfo::tr("Global parameters");
}
QJsonValue GlobalParametersItem::serialize() const
{
    return QJsonObject {
        {IPS_STR("pa"), static_cast<int>(m_pa)}
        , {IPS_STR("duplex"), static_cast<int>(m_duplex)}
        , {IPS_STR("alarm"), static_cast<int>(m_alarm)}
        , {IPS_STR("dgrCall"), static_cast<int>(m_dynamic_group_call)}
        , {IPS_STR("conference"), static_cast<int>(m_conference)}
        , {IPS_STR("grPa"), static_cast<int>(m_group_pa)}
        , {IPS_STR("callerid"), static_cast<int>(m_callerid)}
        , {IPS_STR("ip"), m_ip}
        , {IPS_STR("netmask"), m_netmask}
        , {IPS_STR("gateway"), m_gateway}
    };
}
bool GlobalParametersItem::clear()
{
    m_pa = 60U;
    m_alarm = 70U;
    m_duplex = 50U;
    m_group_pa = 65U;
    m_conference = 40U;
    m_dynamic_group_call = 55U;
    m_callerid = 1000U;
    m_ip = IPS_STR("192.168.10.1");
    m_netmask = IPS_STR("255.255.255.0");
    m_gateway.clear();
    ips::algo::clearList<CidsContainer*>(m_cids);
    return true;
}
bool GlobalParametersItem::deserialize(QJsonObject const& data)
{
    IPS_JSON_EXTRACT_OBJECT(data, g, IPS_STR("Global parameters"))
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("pa"), m_pa)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("duplex"), m_duplex)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("dgrCall"), m_dynamic_group_call)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("alarm"), m_alarm)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("conference"), m_conference)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("grPa"), m_group_pa)
    IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("callerid"), m_callerid)
    IPS_JSON_TO_SIMPLE_STR(g.toObject(), IPS_STR("ip"), m_ip)
    IPS_JSON_TO_SIMPLE_STR(g.toObject(), IPS_STR("netmask"), m_netmask)
    IPS_JSON_TO_SIMPLE_STR(g.toObject(), IPS_STR("gateway"), m_gateway)
    return true;
}
QList<CidsContainer*>& GlobalParametersItem::cids() noexcept
{
    return m_cids;
}
