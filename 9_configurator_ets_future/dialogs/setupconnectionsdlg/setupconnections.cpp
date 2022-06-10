#include <QString>
#include "setupconnections.h"
#include "ui_setupconnections.h"

SetupConnectionsDialog::SetupConnectionsDialog(QWidget *parent, CModeExchange mode)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::SetupConnectionsDialog)
{
    ui->setupUi(this);
	modeConnections = mode;
    instanceNetworkList();
    insatnceEnable();
}

SetupConnectionsDialog::~SetupConnectionsDialog()
{
    delete ui;
}
void SetupConnectionsDialog::insatnceEnable()
{
	bool _is = false;
	if (!ui->m_com_ports->count()){
		_is = true;
	}
    ui->m_com_ports->setDisabled(_is);
}
void SetupConnectionsDialog::instanceNetworkList()
{
	if (modeConnections == RS232Mode){
		ui->l_title->setText(QString::fromLocal8Bit("RS232"));
		ui->l_port->setText(QString::fromLocal8Bit("Choosing ttyUSBx/COMx port"));
		ui->l_info->setText(QString::fromLocal8Bit("* 8 data bits, no flow control, no parity bit, two-way\n data transfer, 1 stop bit, the structure is speed-8-N-1"));
		QList<QSerialPortInfo> _lstPorts = QSerialPortInfo::availablePorts();
		QStandardItemModel* model = new QStandardItemModel(this);
		for (quint8 _i = 0; _i < _lstPorts.count(); _i++)
			model->appendRow(new QStandardItem(_lstPorts.at(_i).portName()));
		QSortFilterProxyModel* proxy1 = new QSortFilterProxyModel();
		proxy1->setSourceModel(model);
		proxy1->sort(0);
		ui->m_com_ports->setModel(proxy1);
	} else {
		ui->l_title->setText(QString::fromLocal8Bit("Network"));
		ui->l_port->setText(QString::fromLocal8Bit("Choosing self network interface"));
		ui->l_info->setText(QString::fromLocal8Bit(""));
		QNetworkInterface* _nt = new QNetworkInterface();
		QList<QHostAddress> _ae = _nt->allAddresses();
		QStandardItemModel* model = new QStandardItemModel(this);
		for(int _i = 0; _i < _ae.count(); _i++){
			if (_ae.at(_i).protocol() != QAbstractSocket::IPv4Protocol) continue;
			if (_ae.at(_i) == QHostAddress::LocalHost) continue;
			if (_ae.at(_i) == QHostAddress::Broadcast) continue;
			model->appendRow(new QStandardItem(IP2Str_Dec(_ae.at(_i).toIPv4Address())));
		}
		QSortFilterProxyModel* proxy1 = new QSortFilterProxyModel();
		proxy1->setSourceModel(model);
		proxy1->sort(0);
		ui->m_com_ports->setModel(proxy1);
	}
}

QString SetupConnectionsDialog::getConnectionsParamStr()
{
	if (ui->m_com_ports->isEnabled()){
		return ui->m_com_ports->currentText();
	} else {
		return QString::fromLocal8Bit("");		
	}
}

void SetupConnectionsDialog::setConnectionsParamStr(QString _str){
	ui->m_com_ports->setCurrentText(_str);
}

QString SetupConnectionsDialog::IP2Str_Dec( unsigned int _IP )
{
	QString _str;
	_str = QObject::tr("%1.%2.%3.%4").arg(QString::number((_IP >> 24) & 0xFF, 10 )).arg(QString::number((_IP >> 16) & 0xFF, 10 )).arg(QString::number((_IP >>  8) & 0xFF, 10 )).arg(QString::number((_IP      ) & 0xFF, 10 ));
	return _str;
} 