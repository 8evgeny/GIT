#include <QString>
#include "debugviewdlg.h"
#include "ui_debugviewdlg.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

DebugViewDialog::DebugViewDialog(QWidget *parent, CModeExchange mode)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::DebugViewDialog)
{
    ui->setupUi(this);
	connect(ui->b_start, SIGNAL(clicked()), this, SLOT(b_start_clicked()));
	connect(ui->b_savelog, SIGNAL(clicked()), this, SLOT(b_savelog_clicked()));
	connect(ui->b_loadlog, SIGNAL(clicked()), this, SLOT(b_loadlog_clicked()));
	connect(ui->cb_liststations, SIGNAL(currentIndexChanged(int)), this, SLOT(cb_liststations_currentIndexChanged(int)));

	globalConfigureStr = QString::fromLocal8Bit("");
	ModeExchange = mode;
	portName = QString::fromLocal8Bit("");
	model = new MsgTableModel;
	ui->tableView_msg->setModel(model); 
	isStart = false;
	serialPort = new QSerialPort(this);
	serialPort->setBaudRate(QSerialPort::Baud115200);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	pTimeTimeouts = new QTimer();
	pTimeTimeouts->setSingleShot(true);
	pTimeTimeouts->setInterval(1000);
	connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
	connect(pTimeTimeouts, SIGNAL(timeout()), this, SLOT(pTimeTimeouts_timeout()));
	udp_socket = new QUdpSocket();
	connect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	LengthAeskey = 0;
	AesModule = new CryptoDialog();

	testTimer = new QTimer(this);
	testTimer->setInterval(1000);
	testTimer->setSingleShot(false);
	connect(testTimer, SIGNAL(timeout()), this, SLOT(testTimer_timeout()));
	RxUDPPort = 12341;
}

DebugViewDialog::~DebugViewDialog()
{
	disconnect(pTimeTimeouts, SIGNAL(timeout()), this, SLOT(pTimeTimeouts_timeout()));
	delete pTimeTimeouts;
	disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
	if(serialPort->isOpen())
		serialPort->close();
	delete serialPort;
	disconnect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	delete udp_socket;
    delete ui;
}

void DebugViewDialog::setTitle(QString _title){
	ui->l_title->setText(_title);
}

void DebugViewDialog::setMode(CModeExchange _mode){
	ModeExchange = _mode;
}

void DebugViewDialog::setRS232Name(QString _portname){
	portName = _portname;
	serialPort->setPortName(portName);
}

void DebugViewDialog::setSelfIP(QString _selfip){
	selfIP = _selfip;
	QString strIP;
	quint32 ip_dec = QHostAddress(selfIP).toIPv4Address();
	OpenStateUdpSocket = udp_socket->bind(QHostAddress(ip_dec), RxUDPPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
//	qDebug() << QObject::tr("DebugViewDialog::RxUDPPort=%1").arg(RxUDPPort);
	ip_dec = qFromBigEndian<quint32>(ip_dec);
	strIP = Data2Hex((unsigned char*)&ip_dec, 4);
	strPrepend = QObject::tr("CONFIG : %1 : ").arg(strIP);
}

void DebugViewDialog::setAesKey(uint8_t* key, uint8_t length_key){
	unsigned char _hex_key[64];
	unsigned char _decode_key[32];
	unsigned char _uc;
	QString _substr;
	QByteArray _ba;
	unsigned int _result;
	bool _ok;
	memcpy(_hex_key, key, length_key*2);
	for(int _j = 0; _j < length_key; _j++){
		_ba.clear();
		_ba.append(_hex_key[_j*2]);
		_ba.append(_hex_key[_j*2+1]);
		_substr =  QString::fromLocal8Bit(_ba);
		_result = _substr.toUInt(&_ok, 16);
		_uc = _result & 0xFF;
		_decode_key[_j] = _uc;
	}
	LengthAeskey = length_key;
	memcpy(AesKey, _decode_key, 16);
	AesModule->setKey(_decode_key, AES128);
}

void DebugViewDialog::b_start_clicked()
{
	if (isStart){
		fnDebugOff();
		isTimeoutAnswer = false;
		pTimeTimeouts->setInterval(1000);
		pTimeTimeouts->start();
		while(!isTimeoutAnswer){
			qApp->processEvents();
		}
		isStart = false;
		ui->b_start->setText(QString::fromLocal8Bit("Start"));
		if (ModeExchange == RS232Mode){
			if(serialPort->isOpen())
				serialPort->close();
			disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
		} else
		if (ModeExchange == NetworkMode){
			disconnect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
		}
	} else {
		if (ModeExchange == RS232Mode){
			if (!serialPort->open(QIODevice::ReadWrite)){
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Serial port not open!!!"));
				return;
			}
			serialPort->clear();
		}
		if (ModeExchange == NetworkMode){
			if (!OpenStateUdpSocket){
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Udp socket is not binding port!!!"));
				return;
			}
		}
		fnDebugOn();
		isStart = true;
		ui->b_start->setText(QString::fromLocal8Bit("Stop"));
	}
}

void DebugViewDialog::testTimer_timeout(){
	QString _dt = QDateTime::currentDateTime().toString(QString::fromLocal8Bit("yyyy.MM.dd hh:mm:ss:zzz"));
	QString _cid = ui->cb_liststations->currentText();
	QString _ip = ui->le_ipstation->text();
	QString _msg = QString::fromLocal8Bit("test");
	appendMsg(_dt, _cid, _ip, _msg);
	ui->tableView_msg->resizeColumnsToContents();
	ui->tableView_msg->resizeRowsToContents();
}

void DebugViewDialog::b_savelog_clicked()
{
    auto file = QFileDialog::getSaveFileName(this, QObject::tr("Save a debug log as...")
                                    , QString::fromLocal8Bit(""), QString::fromLocal8Bit("dbg")); 

    if (file.isEmpty())
        return;

    if (!file.endsWith(IPS_STR(".dbg"))) {
        file = IPS_STR("%1.dbg").arg(file);
    }
	model->save(file);
}

void DebugViewDialog::b_loadlog_clicked(){
    auto const& file = QFileDialog::getOpenFileName(this, QObject::tr("Load debug log...")
                                    , QString::fromLocal8Bit(""), QString::fromLocal8Bit("*.dbg")); 

	if (!file.isEmpty()){
		model->load(file);
	}
}

void DebugViewDialog::setConfigureStr(QString _data){
	int cidName;
	QString _str;
	globalConfigureStr = _data;
	ui->cb_liststations->clear();
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		_str.setNum(cidName);
		_str.prepend(QString::fromLocal8Bit("CID"));
		ui->cb_liststations->addItem(_str);
	}
	QJsonObject srcGlobalParameters = mapObj[QString::fromLocal8Bit("Global parameters")].toObject();
	setAesKey((uint8_t*)srcGlobalParameters[QString::fromLocal8Bit("key AES")].toString().toLocal8Bit().data(), 16);
	RxUDPPort = srcGlobalParameters[QString::fromLocal8Bit("port configuration")].toInt();
}

void DebugViewDialog::cb_liststations_currentIndexChanged(int index){
	Q_UNUSED(index);
	int cidName;
	QString ip;
	QString str;
	QString ExpectedStr = ui->cb_liststations->currentText();
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		str.setNum(cidName);
		str.prepend(QString::fromLocal8Bit("CID"));
		if (str == ExpectedStr){
			str = obj[QString::fromLocal8Bit("ip")].toString();
			ui->le_ipstation->setText(str);
			return;
		}
		
	}
}

QString DebugViewDialog::getIPFromCid(QJsonArray* StationList, QString _cid){
	QJsonObject mapStation;
	int cidName;
	QString str;
	for(int _i = 0; _i < StationList->count(); _i++){
		mapStation = StationList->at(_i).toObject();
		cidName	= mapStation[QString::fromLocal8Bit("cid")].toInt();
		str.setNum(cidName);
		if (_cid.indexOf(str) != -1){
			return mapStation[QString::fromLocal8Bit("ip")].toString();
		}
	}
	return QString::fromLocal8Bit("");
}

void DebugViewDialog::appendMsg(QString _dt, QString _cid, QString _ip, QString _msg){
	MsgStringItem* it = new MsgStringItem();
	it->datetime = _dt;
	it->cid = _cid;
	it->ip = _ip;
	it->msg = _msg;
	model->AppendRow(it); 
	model->MyReset();
}

void DebugViewDialog::serialPort_dataRead()
{
	QByteArray _ba = serialPort->readAll();
	for(int _i = 0; _i < _ba.count(); _i++){
		if (_ba.at(_i) != '\n'){
			if (_ba.at(_i) != '\r')
				rcvStr += QChar::fromLatin1(_ba.at(_i));
		} else {
//			parse_msg();
			QString _dt = QDateTime::currentDateTime().toString(QString::fromLocal8Bit("yyyy.MM.dd hh:mm:ss:zzz"));
			QString _cid = ui->cb_liststations->currentText();
			QString _ip = ui->le_ipstation->text();
			QString _msg = rcvStr;
			appendMsg(_dt, _cid, _ip, _msg);
			ui->tableView_msg->resizeColumnsToContents();
			ui->tableView_msg->resizeRowsToContents();
			rcvStr = QString::fromLocal8Bit("");
		}
	}
}

void DebugViewDialog::pTimeTimeouts_timeout()
{
	isTimeoutAnswer = true;
}

void DebugViewDialog::readPendingDatagrams()
{
	char _tmpbuff[65536];
	qint32 _byte_read = 0; 
	if (udp_socket->pendingDatagramSize()){
		_byte_read = udp_socket->readDatagram((char*)_tmpbuff, 65536); 
		QByteArray _ba = QByteArray(_tmpbuff,_byte_read);
	     processTheDatagram(_ba);
	} 
}

void DebugViewDialog::processTheDatagram(QByteArray dt){
	char enc_buff[1500];
	char dec_buff[1500];
	QString _decStr;
	memset(enc_buff, 0, 1500);
	memset(dec_buff, 0, 1500);
	memcpy(enc_buff, dt.data(), dt.count());
	AesModule->DecodeBuffer((uint8_t*)enc_buff, dt.count(), (uint8_t*)dec_buff);
	_decStr = QString::fromLocal8Bit(dec_buff);
/*	if (_decStr.indexOf(QString::fromLocal8Bit("ACK : ")) != -1){
		rcvStr = _decStr.mid(17);
		parse_msg();
	}*/
	QString _dt = QDateTime::currentDateTime().toString(QString::fromLocal8Bit("yyyy.MM.dd hh:mm:ss:zzz"));
	QString _cid = ui->cb_liststations->currentText();
	QString _ip = ui->le_ipstation->text();
	QString _msg = _decStr;
	appendMsg(_dt, _cid, _ip, _msg);
	ui->tableView_msg->resizeColumnsToContents();
	ui->tableView_msg->resizeRowsToContents();
}

QString DebugViewDialog::Data2Hex(unsigned char* _code, unsigned int _length_byte)
{
	QString Str = QString::fromLocal8Bit("");
	unsigned int _tp;
	QString subStr;
	for(unsigned int _i = 0; _i < _length_byte; _i++){
		_tp = _code[_i];
		subStr.setNum(_tp, 16);
		subStr = subStr.rightJustified(2, QLatin1Char('0'));
		subStr = subStr.toUpper();
		Str += subStr;
	}
	return Str;
}

void DebugViewDialog::fnDebugOn(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint8_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);

	targetIP = ui->le_ipstation->text();
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_DEBUG_ON);
	if (ModeExchange == RS232Mode){
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (ModeExchange == NetworkMode){
		_str.prepend(strPrepend);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), RxUDPPort);
		} else {
			return;
		}
	}
}

void DebugViewDialog::fnDebugOff(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint8_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);

	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_DEBUG_OFF);
	if (ModeExchange == RS232Mode){
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (ModeExchange == NetworkMode){
		_str.prepend(strPrepend);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), RxUDPPort);
		} else {
			return;
		}
	}
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
