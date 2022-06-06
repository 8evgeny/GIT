#include <QString>
#include "communication.h"
#include <iostream>

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
	#pragma GCC diagnostic ignored "-Wsign-conversion"
	#pragma GCC diagnostic ignored "-Wold-style-cast"
//	#define PACKED __attribute__ ((__packed__, aligned(1)))
#elif defined(_MSC_VER)
//	#define PACKED
//	#pragma pack(push,1)
#endif

//максимальная длина строки для предачи в устройство
#define MAX_PARAMETERS_LENGTH	256
//максимальное количество параметров в строке
#define MAX_NUMBER_OF_PARAMETERS	8
//значение максимального времени ожидания ответа от устройства, мс
#define DEFAULT_TIMEOUT_ANSWER		2000
//стандартный полином для таблицы CRC
const unsigned CRC_POLY = 0xEDB88320;
//маска для расчета CRC
const unsigned CRC_MASK = 0xD202EF8D;

//таблица CRC
unsigned table[256];

CommunicationDialog::CommunicationDialog(QWidget *parent)
    : QObject(parent)
{
	serialPort = new QSerialPort(this);
	serialPort->setBaudRate(QSerialPort::Baud115200);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	pTimeTimeouts = new QTimer();
	pTimeTimeouts->setSingleShot(true);
	pTimeTimeouts->setInterval(DEFAULT_TIMEOUT_ANSWER);
	connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
	connect(pTimeTimeouts, SIGNAL(timeout()), this, SLOT(pTimeTimeouts_timeout()));
	rcvStr = QString::fromLocal8Bit("");
	udp_socket = new QUdpSocket();
	connect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	OpenStateUdpSocket = false;
	LengthAeskey = 0;
	AesModule = new CryptoDialog();
	RxUDPPort = 12341;
}

CommunicationDialog::~CommunicationDialog()
{
	disconnect(pTimeTimeouts, SIGNAL(timeout()), this, SLOT(pTimeTimeouts_timeout()));
	delete pTimeTimeouts;
	disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
	if(serialPort->isOpen())
		serialPort->close();
	delete serialPort;
	disconnect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	delete udp_socket;
	delete AesModule;
}

void CommunicationDialog::setMode(CModeExchange _mode){
	modeExchange = _mode;
}

void CommunicationDialog::setSelfIP(QString _ip){
	QString strIP;
	selfIP = _ip;
	quint32 ip_dec = QHostAddress(selfIP).toIPv4Address();
	OpenStateUdpSocket = udp_socket->bind(QHostAddress(ip_dec), RxUDPPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
//	qDebug() << QObject::tr("CommunicationDialog::RxUDPPort=%1").arg(RxUDPPort);
	ip_dec = qFromBigEndian<quint32>(ip_dec);
	strIP = Data2Hex((unsigned char*)&ip_dec, 4);
	strPrepend = QObject::tr("CONFIG : %1 : ").arg(strIP);
}

void CommunicationDialog::setTargetIP(QString _ip){
	targetIP = _ip;
}

void CommunicationDialog::setRxUDPPort(uint32_t _port)
{
	RxUDPPort = _port;
}

void CommunicationDialog::setAesKey(uint8_t* key, uint8_t length_key){
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

void CommunicationDialog::setRs232PortName(QString portName){
	PortName = portName;
	serialPort->setPortName(PortName);
}

void CommunicationDialog::serialPort_dataRead()
{
	QByteArray _ba = serialPort->readAll();
	for(int _i = 0; _i < _ba.count(); _i++){
/*		if (_ba.at(_i) != '\n'){
			if (_ba.at(_i) != '\r')
				rcvStr += QChar::fromLatin1(_ba.at(_i));
		} else {
			parse_msg();
		}
		*/
		rcvStr += QChar::fromLatin1(_ba.at(_i));
		if (_ba.at(_i) == '\n') {
			rcvMsgList.append(rcvStr);
			rcvStr = QString::fromLocal8Bit("");
		}
	}
}

void CommunicationDialog::pTimeTimeouts_timeout()
{
	isTimeoutAnswer = true;
}

void CommunicationDialog::readPendingDatagrams()
{
	char _tmpbuff[65536];
	qint32 _byte_read = 0; 
	if (udp_socket->pendingDatagramSize()){
		_byte_read = udp_socket->readDatagram((char*)_tmpbuff, 65536); 
		QByteArray _ba = QByteArray(_tmpbuff,_byte_read);
	     processTheDatagram(_ba);
	} 
}

void CommunicationDialog::processTheDatagram(QByteArray dt){
	char enc_buff[1500];
	char dec_buff[1500];
	QString _decStr;
	memset(enc_buff, 0, 1500);
	memset(dec_buff, 0, 1500);
	memcpy(enc_buff, dt.data(), dt.count());
	_decStr = QString::fromLocal8Bit(enc_buff);
	if (_decStr.indexOf(QString::fromLocal8Bit("ACK : ")) != -1){
		AesModule->DecodeBuffer((uint8_t*)&enc_buff[17], dt.count()-17, (uint8_t*)dec_buff);
		rcvStr = QString::fromLocal8Bit(dec_buff);
		rcvMsgList.append(rcvStr);
/*			QString _fl1 = QString::fromLocal8Bit("rcv.txt");
			QFile* _file1 = new QFile(_fl1);
			if (_file1->open(QIODevice::WriteOnly) | QIODevice::Text){
				_file1->write(rcvStr.toLocal8Bit().data(), _decStr.length());
				_file1->close();
			}
			delete _file1;*/
//		parse_msg();
		rcvStr = QString::fromLocal8Bit("");
	}
}

void CommunicationDialog::parse_msg(QString _msg)
{
	QString* SubStr;
	QString _str;
	int pos = 0;
	if (lstRcvAnswer.count()){
		for(int _i = 0; _i < lstRcvAnswer.count(); _i++)
			delete lstRcvAnswer.at(_i);
		lstRcvAnswer.clear();
	}
	_str = _msg;
	pos = _str.indexOf(QString::fromLocal8Bit(" : "));
	while(pos != -1){
		SubStr = new QString(_str.mid(0, pos));
		_str = _str.mid(pos+3, -1);
		lstRcvAnswer.append(SubStr);
		pos = _str.indexOf(QString::fromLocal8Bit(" : "));
	}
	SubStr = new QString(_str);
	lstRcvAnswer.append(SubStr);
	isRcvStr = true;
//	rcvStr += QString::fromLocal8Bit("\r\n");
	add2Log(_msg, false);
//	rcvStr = QString::fromLocal8Bit("");
}

QString CommunicationDialog::Data2Hex(unsigned char* _code, unsigned int _length_byte)
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

void CommunicationDialog::crc32(){
	unsigned int i, j, r;
	for (i = 0; i < 256; i++){
		for (r = i, j = 8; j; j--)
			r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
		table[i] = r;
	}
}

unsigned CommunicationDialog::ProcessCRC(char* pData, unsigned int nLen){
	unsigned char* pdata = reinterpret_cast<unsigned char*>(pData);
	unsigned int crc = 0;
	unsigned int index;
	if (nLen){
		while (nLen--){
			index = (crc & 0xFF) ^ *pdata++;
			crc = table[index] ^ crc >> 8;
			crc ^= CRC_MASK;
		}
	}
	return crc;
}

QString CommunicationDialog::getLastErrorString(){
	return lastErrorStr;
}

bool CommunicationDialog::fnGetCFGUI(unsigned int* _type){
	QString _str;
	bool _ok;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint8_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	QByteArray _ba;
	_str = QString::fromLocal8Bit(CMD_GET_CFGUI);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*
		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("CFGUI")) == -1){
				isRcvStr = false;
			} else {
				_str = lstRcvAnswer.at(1)->mid(0, 2);
				*_type = _str.toUInt(&_ok);
				pTimeTimeouts->stop();
				pTimeTimeouts->stop();
				return true;
				break;
			}
		}
*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("CFGUI")) == -1) {
			}
			else {
				_str = lstRcvAnswer.at(1)->mid(0, 2);
				*_type = _str.toUInt(&_ok);
				pTimeTimeouts->stop();
				pTimeTimeouts->stop();
				return true;
				break;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"1"<<std::endl;
		add2Log(lastErrorStr, false);
		return false;
	}
	pTimeTimeouts->stop();
	return false;
}

bool CommunicationDialog::setRqSetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size){
QString _str;
unsigned int _tmp, _repeaters;
char tsrc_buff[1400];
char tdst_buff[1400];
uint32_t _size_enc_buff;
memset(tsrc_buff, 0, 1400);
memset(tdst_buff, 0, 1400);
QByteArray _ba;
	lastErrorStr = QString::fromLocal8Bit("");
	_repeaters = 3;
	while(_repeaters){
		pTimeTimeouts->stop();
		_str = QString::fromLocal8Bit(CMD_SET_BLOCK);
		_str += QString::fromLocal8Bit(" : ");
		_tmp = _type_block;
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
		_str += QString::fromLocal8Bit(" : ");
		_tmp = _addr;
		_tmp = qFromBigEndian<quint32>(_tmp);
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
		_str += QString::fromLocal8Bit(" : ");
		_str += Data2Hex(_data, _size);
		_str += QString::fromLocal8Bit(" : ");
		crc32();
		_tmp = ProcessCRC(reinterpret_cast<char*>(_data), _size);
		_tmp = qFromBigEndian<quint32>(_tmp);
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
		_str += QLatin1Char('\r');
		_str += QLatin1Char('\n');
		rcvStr = QString::fromLocal8Bit("");
		if (modeExchange == RS232Mode){
			add2Log(_str, true);
			serialPort->clear();
			serialPort->write(_str.toLocal8Bit().data(), _str.length());
		} else
		if (modeExchange == NetworkMode){
//			_str.prepend(strPrepend);
			add2Log(_str, true);
			memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
			_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
			if (_size_enc_buff){
				_str = strPrepend;
				_ba = _str.toLocal8Bit();
				_ba.append((char*)&tdst_buff[0], _size_enc_buff);
				udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//				udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
			} else {
				lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				return false;
			}
		}
		pTimeTimeouts->start();
		isTimeoutAnswer = false;
		isRcvStr = false;
		isCancel = false;
		while(!isTimeoutAnswer){
			qApp->processEvents();
			if (isCancel){
				pTimeTimeouts->stop();
				return true;
			}
/*			if (isRcvStr){
				if (lstRcvAnswer.count()){
					if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Set block data")) == -1){
						lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
						add2Log(lastErrorStr, false);
						isRcvStr = false;
					} else {
						if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
							lastErrorStr = QObject::tr("Error execute cmd(Set Block)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
							add2Log(lastErrorStr, false);
							pTimeTimeouts->stop();
							return false;
						} else {
							pTimeTimeouts->stop();
							return true;
						}
					}
				}
			}*/
			if (rcvMsgList.count()) {
				_str = rcvMsgList.at(0);
				rcvMsgList.removeAt(0);
				parse_msg(_str);
				if (lstRcvAnswer.count()) {
					if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Set block data")) == -1) {
						lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
						add2Log(lastErrorStr, false);
						isRcvStr = false;
					}
					else {
						if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
							lastErrorStr = QObject::tr("Error execute cmd(Set Block)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
							add2Log(lastErrorStr, false);
							pTimeTimeouts->stop();
							return false;
						}
						else {
							pTimeTimeouts->stop();
							return true;
						}
					}
				}
			}
		}
		if (isTimeoutAnswer){
			_repeaters--;
		}
	}
	if (isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"2"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return false;
}

bool CommunicationDialog::setRqGetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size){
	int _j;
	QString _str, _substr;
	quint32 _tmp, _result;
	bool _ok;
	unsigned char _uc, _repeaters;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint32_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;

	lastErrorStr = QString::fromLocal8Bit("");
	_repeaters = 3;
	while(_repeaters){
		pTimeTimeouts->stop();
		_str = QString::fromLocal8Bit(CMD_GET_BLOCK);
		_str += QString::fromLocal8Bit(" : ");
		_tmp = _type_block;
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
		_str += QString::fromLocal8Bit(" : ");
		_tmp = _addr;
		_tmp = qFromBigEndian<quint32>(_tmp);
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
		_str += QString::fromLocal8Bit(" : ");
		_tmp = _size;
		_tmp = qFromBigEndian<quint32>(_tmp);
		_str += Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
		_str += QLatin1Char('\r');
		_str += QLatin1Char('\n');
		rcvStr = QString::fromLocal8Bit("");
		if (modeExchange == RS232Mode){
			add2Log(_str, true);
			serialPort->clear();
			serialPort->write(_str.toLocal8Bit().data(), _str.length());
		} else
		if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
			add2Log(_str, true);
			memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
			_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
			if (_size_enc_buff){
				_str = strPrepend;
				_ba = _str.toLocal8Bit();
				_ba.append((char*)&tdst_buff[0], _size_enc_buff);
				udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
			} else {
				lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				return false;
			}
		}
		pTimeTimeouts->start();
		isTimeoutAnswer = false;
		isRcvStr = false;
		isCancel = false;
		while(!isTimeoutAnswer){
			qApp->processEvents();
			if (isCancel){
				pTimeTimeouts->stop();
				return true;
			}
/*			if (isRcvStr){
				if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Get block data")) == -1){
					lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					isRcvStr = false;
				} else {
					_str =(*lstRcvAnswer.at(1));
					for(_j = 0; _j < _str.length()/2; _j++){
						_substr = _str.mid(_j*2, 2);
						_result = _substr.toUInt(&_ok, 16);
						_uc = _result & 0xFF;
						_data[_j] = _uc;
					}
					pTimeTimeouts->stop();
					return true;
				}
			}*/
			if (rcvMsgList.count()) {
				_str = rcvMsgList.at(0);
				rcvMsgList.removeAt(0);
				parse_msg(_str);
				if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Get block data")) == -1) {
					lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					isRcvStr = false;
				}
				else {
					_str = (*lstRcvAnswer.at(1));
					for (_j = 0; _j < _str.length() / 2; _j++) {
						_substr = _str.mid(_j * 2, 2);
						_result = _substr.toUInt(&_ok, 16);
						_uc = _result & 0xFF;
						_data[_j] = _uc;
					}
					pTimeTimeouts->stop();
					return true;
				}
			}
		}
		if (isTimeoutAnswer){
			_repeaters--;
		}
	}
	if (isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"3"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return false;
}

bool CommunicationDialog::fnConfigOn(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint8_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;

	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_CONFIG_ON);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config on")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Config On)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config on")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Config On)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"4"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnConfigOff(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint8_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;

	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_CONFIG_OFF);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			return true;
		}
/*		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config off")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Config Off)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config off")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Config Off)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"5"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnConfigSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			_pSysBlockHdr->parts[_i].Size = _size;
			_pSysBlockHdr->parts[_i].CRC = _crc;
			_pSysBlockHdr->parts[_i].Ver = _ver;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			*_size = _pSysBlockHdr->parts[_i].Size;
			*_crc = _pSysBlockHdr->parts[_i].CRC;
			*_ver = _pSysBlockHdr->parts[_i].Ver;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigSetDataSize(unsigned int _size){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			_pSysBlockHdr->parts[_i].Size = _size;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigGetDataSize(unsigned int* _data_size){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			*_data_size = _pSysBlockHdr->parts[_i].Size;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigSetBlock(char* _data,  unsigned int _addr, unsigned int _size){
	if (setRqSetBlock(EntryType_CNF, _addr, reinterpret_cast<unsigned char*>(&_data[0]), _size) == false){
		return false;
	}
	return true;
}

bool CommunicationDialog::fnConfigGetBlock(char* _data, unsigned int _addr, unsigned int _size){
	if (setRqGetBlock(EntryType_CNF, _addr, reinterpret_cast<unsigned char*>(&_data[0]), _size) == false){
		return false;
	}
	return true;
}

bool CommunicationDialog::fnConfigSetCRC(unsigned int _crc){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			_pSysBlockHdr->parts[_i].CRC = _crc;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigGetCRC(unsigned int* _crc){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			*_crc = _pSysBlockHdr->parts[_i].CRC;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigStartWrite(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint32_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;

	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_CONFIG_START_WRITE);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
//	pTimeTimeouts->setInterval(5*60*1000);
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config result")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Config Start Write)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
//					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Config result")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Config Start Write)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					//					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"6"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnConfigSetVersion(int _version){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			_pSysBlockHdr->parts[_i].Ver = _version;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnConfigGetVersion(int* _version){
	uint32_t* _pVer = reinterpret_cast<uint32_t*>(_version);
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_CNF){
			*_pVer = _pSysBlockHdr->parts[_i].Ver;
		}
	}
	return true;
}

bool CommunicationDialog::fnSetKeyAes(QString _key){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	unsigned char _dst_buff[16];
	QString _substr;
	bool _ok;
	unsigned int _result;
	int _i;
	unsigned char arr_sysblock[256];
	for(int _j = 0; _j < 16; _j++){
		_substr = _key.mid(_j*2, 2);
		_result = _substr.toUInt(&_ok, 16);
		_dst_buff[_j] = _result & 0xFF;
	}
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 16; _i++){
		_pSysBlockHdr->KeyAES[_i] = _dst_buff[_i];
	}
	crc32();
	_pSysBlockHdr->parts[EntryType_SB].CRC = 0;
	_pSysBlockHdr->parts[EntryType_SB].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnGetKeyAes(QString* _key){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	QByteArray _ba;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	*_key = Data2Hex(&_pSysBlockHdr->KeyAES[0], 16);
	*_key = _key->toUpper();
	return true;
}

bool CommunicationDialog::fnSetSN(QString _sn){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	QByteArray _ba = _sn.toLocal8Bit();
	int _i;
	unsigned char arr_sysblock[512];
	char _c;
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 12; _i++){
		_c = _ba.at(_i);
		_pSysBlockHdr->SN[_i] = _c;
	}
	crc32();
	_pSysBlockHdr->parts[EntryType_SB].CRC = 0;
	_pSysBlockHdr->parts[EntryType_SB].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnGetSN(QString* _sn){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	QByteArray _ba;
	int _i;
	unsigned char arr_sysblock[256];
	unsigned char* _pUChar;
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 12; _i++){
		_pUChar = &_pSysBlockHdr->SN[_i];
		_ba.append(*(reinterpret_cast<char*>(_pUChar)));
	}
	*_sn = QString::fromLocal8Bit(_ba);
	return true;
}

bool CommunicationDialog::fnFwwareOn(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint32_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;

	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_FMWARE_ON);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*		if (isRcvStr){
			if((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware on")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Fmware On)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware on")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Fmware On)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"7"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnFwwareOff(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint32_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;
	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_FMWARE_OFF);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware off")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Fmware Off)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware off")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Fmware Off)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"8"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnFwwareSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			_pSysBlockHdr->parts[_i].Size = _size;
			_pSysBlockHdr->parts[_i].CRC = _crc;
			_pSysBlockHdr->parts[_i].Ver = _ver;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			*_size = _pSysBlockHdr->parts[_i].Size;
			*_crc = _pSysBlockHdr->parts[_i].CRC;
			*_ver = _pSysBlockHdr->parts[_i].Ver;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareSetDataSize(unsigned int _size){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			_pSysBlockHdr->parts[_i].Size = _size;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareGetDataSize(unsigned int* _data_size){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			*_data_size = _pSysBlockHdr->parts[_i].Size;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareSetBlock(char* _data,  unsigned int _addr, unsigned int _size){
	return setRqSetBlock(EntryType_FW, _addr, reinterpret_cast<unsigned char*>(&_data[0]), _size);
}

bool CommunicationDialog::fnFwwareGetBlock(char* _data, unsigned int _addr, unsigned int _size){
	return setRqGetBlock(EntryType_FW, _addr, reinterpret_cast<unsigned char*>(&_data[0]), _size);
}

bool CommunicationDialog::fnFwwareSetCRC(unsigned int _crc){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			_pSysBlockHdr->parts[_i].CRC = _crc;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareGetCRC(unsigned int* _crc){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			*_crc = _pSysBlockHdr->parts[_i].CRC;
		}
	}
	return true;
}

bool CommunicationDialog::fnFwwareStartWrite(){
	QString _str;
	char tsrc_buff[1400];
	char tdst_buff[1400];
	uint32_t _size_enc_buff;
	memset(tsrc_buff, 0, 1400);
	memset(tdst_buff, 0, 1400);
	QByteArray _ba;
	lastErrorStr = QString::fromLocal8Bit("");
	isTimeoutAnswer = false;
	isRcvStr = false;
	_str = QString::fromLocal8Bit(CMD_FMWARE_START_WRITE);
	if (modeExchange == RS232Mode){
		add2Log(_str, true);
		serialPort->clear();
		serialPort->write(_str.toLocal8Bit().data(), _str.length());
	} else
	if (modeExchange == NetworkMode){
//		_str.prepend(strPrepend);
		add2Log(_str, true);
		memcpy(tsrc_buff, _str.toLocal8Bit().data(), _str.length());
		_size_enc_buff = AesModule->EncodeBuffer((uint8_t*)&tsrc_buff[0], _str.length(), (uint8_t*)&tdst_buff[0]);
		if (_size_enc_buff){
			_str = strPrepend;
			_ba = _str.toLocal8Bit();
			_ba.append((char*)&tdst_buff[0], _size_enc_buff);
			udp_socket->writeDatagram(_ba.constData(), _ba.count(), QHostAddress(targetIP), RxUDPPort);
//			udp_socket->writeDatagram((char*)&tdst_buff[0], _size_enc_buff, QHostAddress(targetIP), CONFIG_PORT);
		} else {
			lastErrorStr = QObject::tr("Error encode data%1").arg(QString::fromLocal8Bit("\r\n"));
			add2Log(lastErrorStr, false);
			return false;
		}
	}
//	pTimeTimeouts->setInterval(5*60*1000);
	pTimeTimeouts->start();
	isCancel = false;
	while(!isTimeoutAnswer){
		qApp->processEvents();
		if (isCancel){
			pTimeTimeouts->stop();
			return true;
		}
/*		if (isRcvStr){
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware start")) == -1){
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			} else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1){
					lastErrorStr = QObject::tr("Error execute cmd(Fmware Start Write)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}*/
		if (rcvMsgList.count()) {
			_str = rcvMsgList.at(0);
			rcvMsgList.removeAt(0);
			parse_msg(_str);
			if ((*lstRcvAnswer.at(0)).indexOf(QString::fromLocal8Bit("Fmware start")) == -1) {
				lastErrorStr = QObject::tr("Incorrect answer from device!!!\nAnswer from device:\n%1%2").arg(*lstRcvAnswer.at(0)).arg(QString::fromLocal8Bit("\r\n"));
				add2Log(lastErrorStr, false);
				isRcvStr = false;
			}
			else {
				if ((*lstRcvAnswer.at(1)).indexOf(QString::fromLocal8Bit("OK")) == -1) {
					lastErrorStr = QObject::tr("Error execute cmd(Fmware Start Write)\nAnswer from device:\n%1 : %2%3").arg(*lstRcvAnswer.at(0)).arg(*lstRcvAnswer.at(1)).arg(QString::fromLocal8Bit("\r\n"));
					add2Log(lastErrorStr, false);
					pTimeTimeouts->stop();
					return false;
				}
				pTimeTimeouts->stop();
				return true;
			}
		}
	}
	if(isTimeoutAnswer){
		lastErrorStr = QObject::tr("Timeout answer detected!!!%1").arg(QString::fromLocal8Bit("\r\n"));
        std::cout<<"9"<<std::endl;
        add2Log(lastErrorStr, false);
		return false;
	}
	return true;
}

bool CommunicationDialog::fnFmwareSetVersion(int _version){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			_pSysBlockHdr->parts[_i].Ver = _version;
		}
	}
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_SB){
			crc32();
			_pSysBlockHdr->parts[_i].CRC = 0;
			_pSysBlockHdr->parts[_i].CRC = ProcessCRC((char*)&arr_sysblock[0], sizeof(SystemBlockHdr));
		}
	}
//записываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqSetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqSetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	return true;
}

bool CommunicationDialog::fnFmwareGetVersion(int* _version){
	SystemBlockHdr* _pSysBlockHdr;
	int _rc, _rd;
	_rc = sizeof(SystemBlockHdr) / CONFIG_BLOCK_SIZE_RS232;
	_rd = sizeof(SystemBlockHdr) % CONFIG_BLOCK_SIZE_RS232;
	int _i;
	unsigned char arr_sysblock[256];
//вычитываем системный блок
	for(_i = 0; _i < _rc; _i++){
		if (setRqGetBlock(EntryType_SB, _i*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_i*CONFIG_BLOCK_SIZE_RS232]), CONFIG_BLOCK_SIZE_RS232) == false){
			return false;
		}
	}
	if (_rd){
		if (setRqGetBlock(EntryType_SB, _rc*CONFIG_BLOCK_SIZE_RS232, reinterpret_cast<unsigned char*>(&arr_sysblock[_rc*CONFIG_BLOCK_SIZE_RS232]), _rd) == false){
			return false;
		}
	}
	_pSysBlockHdr = reinterpret_cast<SystemBlockHdr*>(&arr_sysblock[0]);
	for(_i = 0; _i < 8; _i++){
		if (_pSysBlockHdr->parts[_i].Type == EEPROM_BLOCK_TYPE_FW){
			*_version = _pSysBlockHdr->parts[_i].Ver;
		}
	}
	return true;
}

void CommunicationDialog::cancel_transactions(){
	isCancel = true;
}

void CommunicationDialog::add2Log(QString _str, bool _isTx){
	LogStr = _str;
	if (_isTx){
		LogStr.prepend(QString::fromLocal8Bit("TX: "));
		LogTxStr = LogStr;
	} else {
		LogStr.prepend(QString::fromLocal8Bit("RX: "));
		LogRxStr = LogStr;
	}
#ifdef __IS_WRITE_LOG__
	QString _fl = QString::fromLocal8Bit("Log.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::Append | QIODevice::Text)){
        return;
	}
	_file->write(LogStr.toLocal8Bit());
	_file->close();
	delete _file;
#endif
}

QString CommunicationDialog::getLogTxStr(){
	return LogTxStr;
}

QString CommunicationDialog::getLogRxStr(){
	return LogRxStr;
}

bool CommunicationDialog::Open(){
	if (modeExchange == RS232Mode){
		if (!serialPort->open(QIODevice::ReadWrite)){
			lastErrorStr = QObject::tr("Serial port not open!!!");
			return false;
		}
		serialPort->clear();
	}
	if (modeExchange == NetworkMode){
		if (!OpenStateUdpSocket){
			lastErrorStr = QObject::tr("Udp socket is not binding port!!!");
			return false;
		}
	}
	return true;
}

void CommunicationDialog::Close(){
	if (modeExchange == RS232Mode){
		if(serialPort->isOpen())
			serialPort->close();
		disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialPort_dataRead()));
	} else
	if (modeExchange == NetworkMode){
		disconnect(udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	}
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
//	#undef PACKED
#elif defined(_MSC_VER)
//	#pragma pack(pop)
//	#undef PACKED
#endif
