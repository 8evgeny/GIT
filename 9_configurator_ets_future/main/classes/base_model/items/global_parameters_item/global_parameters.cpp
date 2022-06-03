#include <ctime>
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
	ui->m_key_aes->setText(keyaes());    
    ui->m_ip_doc->setValidator(new IpValidator);
    ui->m_ip_doc->setText(ip_doc());
    ui->m_port_log_doc->setValidator(new QIntValidator(1000, 32767, ui->m_port_log_doc));
//    ui->m_port_log_doc->setText(convert::toString(2020U));
    ui->m_port_log_doc->setText(convert::toString(m_port_log));
    ui->m_port_rtp_doc->setValidator(new QIntValidator(1000, 32767, ui->m_port_rtp_doc));
//    ui->m_port_rtp_doc->setText(convert::toString(10000U));
    ui->m_port_rtp_doc->setText(convert::toString(m_port_rtp));
    ui->m_port_cnf->setValidator(new QIntValidator(1000, 32767, ui->m_port_cnf));
//    ui->m_port_cnf->setText(convert::toString(12341U));
    ui->m_port_cnf->setText(convert::toString(m_port_cnf));
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
	QObject::connect(ui->m_key_aes, &QLineEdit::editingFinished, this, [this] {m_keyaes = ui->m_key_aes->text();});
	QObject::connect(ui->m_ip_doc, &QLineEdit::editingFinished, this, [this] {editingIP(ui->m_ip_doc, m_ip_doc);});
	QObject::connect(ui->m_port_log_doc, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
	QObject::connect(ui->m_port_rtp_doc, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
	QObject::connect(ui->m_port_cnf, &QLineEdit::textChanged, this, &GlobalParametersItem::dataChanged);
	QObject::connect(ui->b_genaes, &QPushButton::clicked, this, &GlobalParametersItem::b_genaes_clicked);
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
	m_port_log = convert::toUint(ui->m_port_log_doc->text());
	m_port_rtp = convert::toUint(ui->m_port_rtp_doc->text());
	m_port_cnf = convert::toUint(ui->m_port_cnf->text());
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
	ui->l_message->setVisible(false);
	ui->m_alarm->setVisible(false);
	ui->l_conference->setVisible(false);
	ui->m_dynamic_group_call->setVisible(false);
	ui->l_pa->setVisible(false);
	ui->m_pa->setVisible(false);
    updateInterface();
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
		, {IPS_STR("key AES"), m_keyaes}
		, {IPS_STR("ip doc"), m_ip_doc}
		, {IPS_STR("port log doc"), static_cast<int>(m_port_log)}
		, {IPS_STR("port rtp doc"), static_cast<int>(m_port_rtp)}
		, {IPS_STR("port configuration"), static_cast<int>(m_port_cnf)}

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
    m_keyaes.clear();
	m_ip_doc = IPS_STR("192.168.10.1");
	m_port_log = 2020U;
	m_port_rtp = 10000U;
	m_port_cnf = 12341U;
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
        IPS_JSON_TO_SIMPLE_STR(g.toObject(), IPS_STR("key AES"), m_keyaes)
        IPS_JSON_TO_SIMPLE_STR(g.toObject(), IPS_STR("ip doc"), m_ip_doc)
        IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("port log doc"), m_port_log)
        IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("port rtp doc"), m_port_rtp)
        IPS_JSON_TO_SIMPLE_UINT(g.toObject(), IPS_STR("port configuration"), m_port_cnf)
    return true;
}

void GlobalParametersItem::updateInterface()
{
    ui->m_pa->setText(convert::toString(m_pa));
    ui->m_duplex->setText(convert::toString(m_duplex));
    ui->m_alarm->setText(convert::toString(m_alarm));
    ui->m_port_log_doc->setText(convert::toString(m_port_log));
    ui->m_port_rtp_doc->setText(convert::toString(m_port_rtp));
    ui->m_port_cnf->setText(convert::toString(m_port_cnf));
}

QList<CidsContainer*>& GlobalParametersItem::cids() noexcept
{
    return m_cids;
}

void GlobalParametersItem::b_genaes_clicked()
{
	QString rnd;
    QString _str;
    time_t r_time = time(NULL);
	srand(*(reinterpret_cast<unsigned int*>(&r_time)));
	int rnd_val;
	for(int _i = 0; _i < 16; _i++){
		rnd_val = rand() % 255;
		_str.setNum(rnd_val, 16);
		_str = _str.rightJustified(2, QLatin1Char('0'), true);
		rnd += _str;
	}
	rnd = rnd.toUpper();
	ui->m_key_aes->setText(rnd);
	emit ui->m_key_aes->editingFinished();
}
