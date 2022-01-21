#include <QString>
#include "network_configuring.h"
#include "ui_network_configuring.h"

NetworkConfiguringDialog::NetworkConfiguringDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::NetworkConfiguringDialog)
{
    ui->setupUi(this);
    instanceNetworkList();
    insatnceEnable();
	connect(ui->b_open, SIGNAL(clicked()), this, SLOT(b_open_clicked()));
	connect(ui->b_send, SIGNAL(clicked()), this, SLOT(b_send_clicked()));
}
NetworkConfiguringDialog::~NetworkConfiguringDialog()
{
	delete dlgCidList;
    delete ui;
}
void NetworkConfiguringDialog::insatnceEnable()
{
	bool _is = false;
	if (!ui->m_com_ports->count()){
		_is = true;
	}
    ui->m_com_ports->setDisabled(_is);
	ui->b_open->setDisabled(_is);
	ui->b_send->setDisabled(_is);
	ui->m_settings->setDisabled(_is);
	QIcon* _icn = new QIcon(QIcon(IPS_STR(":/svg/status_ok.svg")));
	QToolButton* _tb = new QToolButton();
	_tb->setIcon(*_icn);
	ui->tableWidget->setCellWidget(0, 0, reinterpret_cast<QWidget*>(_tb));
	_icn = new QIcon(QIcon(IPS_STR(":/svg/status_error.svg")));
	_tb = new QToolButton();
	_tb->setIcon(*_icn);
	ui->tableWidget->setCellWidget(1, 0, reinterpret_cast<QWidget*>(_tb));
	ui->tableWidget->resizeColumnsToContents();
	ui->tableWidget->resizeRowsToContents();
}
void NetworkConfiguringDialog::instanceNetworkList()
{
	QNetworkInterface* _nt = new QNetworkInterface();
	QList<QHostAddress> _ae = _nt->allAddresses();
	for(int _i = 0; _i < _ae.count(); _i++){
		if (_ae.at(_i).protocol() != QAbstractSocket::IPv4Protocol) continue;
		if (_ae.at(_i) == QHostAddress::LocalHost) continue;
		if (_ae.at(_i) == QHostAddress::Broadcast) continue;
		ui->m_com_ports->insertItem(-1, IP2Str_Dec(_ae.at(_i).toIPv4Address()));
//		ui->m_com_ports->insertItem(-1, "111");
	}
}

void NetworkConfiguringDialog::b_open_clicked()
{
    if (dlgCidList->exec() == QDialog::Accepted) {               
		ui->m_settings->setText(dlgCidList->current());
    }
}

void NetworkConfiguringDialog::b_send_clicked()
{
}

void NetworkConfiguringDialog::setCidList(QStringList _cidList)
{
	dlgCidList = new StringListDialog(QObject::tr("Destiations"), _cidList);
}

void NetworkConfiguringDialog::setConfigureStr(QString _data)
{
	QString _fl = QString::fromLocal8Bit("1.txt");
	QString _dd = QString::fromLocal8Bit("Hello world");
		QFile* _file = new QFile(_fl);

    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->write(_data.toLocal8Bit());
	_file->close();
	delete _file;
}

QString NetworkConfiguringDialog::IP2Str_Dec( unsigned int _IP )
{
	Q_UNUSED(_IP);
	QString _str;
	_str = QObject::tr("%1.%2.%3.%4").arg(QString::number((_IP >> 24) & 0xFF, 10 )).arg(QString::number((_IP >> 16) & 0xFF, 10 )).arg(QString::number((_IP >>  8) & 0xFF, 10 )).arg(QString::number((_IP      ) & 0xFF, 10 ));
	return _str;
} 
