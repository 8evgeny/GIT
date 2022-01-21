#include <QString>
#include <QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_communication.h"

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

Test_Communication::Test_Communication(QWidget *parent)
    : QObject(parent)
{
	QString key = QString::fromLocal8Bit("000102030405060708090A0B0C0D0E0F");
	pCommDlg = new (CommunicationDialog);
	pCommDlg->setMode(NetworkMode);
	pCommDlg->setSelfIP(QString::fromLocal8Bit("127.0.0.1"));
	pCommDlg->setTargetIP(QString::fromLocal8Bit("127.0.0.1"));
	pCommDlg->setAesKey((uint8_t*)key.toLocal8Bit().data(), 16);
	pCommDlg->Open();
	connect(this, SIGNAL(sigCancel()), pCommDlg, SLOT(cancel_transactions()), Qt::QueuedConnection);
}

Test_Communication::~Test_Communication()
{
	disconnect(this, SIGNAL(sigCancel()), pCommDlg, SLOT(cancel_transactions()));
	pCommDlg->Close();
	delete pCommDlg;
}

void Test_Communication::CheckCRC32(){
	char _src_buff[] = {0x31, 0x32, 0x33, 0x34, 0x35};
	quint32 crc_src_buff = 0xCBF53A1C;
	quint32 calc_src_buff;
	pCommDlg->crc32();
	calc_src_buff = pCommDlg->ProcessCRC(_src_buff, 5);
	QCOMPARE(calc_src_buff, crc_src_buff); 
}

void Test_Communication::add2Log(){
	QString _str;
	pCommDlg->add2Log(QString::fromLocal8Bit("123"), true);
	_str = QString::fromLocal8Bit("TX: 123");
	QCOMPARE(pCommDlg->getLogTxStr(), _str);
	pCommDlg->add2Log(QString::fromLocal8Bit("123"), false);
	_str = QString::fromLocal8Bit("RX: 123");
	QCOMPARE(pCommDlg->getLogRxStr(), _str);
}

void Test_Communication::Data2Hex(){
	unsigned char _test_array[] = {0x01,0x02, 0x03, 0x04, 0x05};
	QString _str = pCommDlg->Data2Hex(_test_array, 5);
	QCOMPARE(_str, QString::fromLocal8Bit("0102030405"));
}

void Test_Communication::fnGetCFGUI(){
	unsigned int _type;
	QString TxStr;
	pCommDlg->fnGetCFGUI(&_type);
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, QString::fromLocal8Bit("TX: DEVICE GET CFGUI\r\n")); 
}

void Test_Communication::fnConfigOn(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_CONFIG_ON);
	pCommDlg->fnConfigOn();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnConfigOff(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_CONFIG_OFF);
	pCommDlg->fnConfigOff();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnConfigSetBlock(){
	unsigned char _test_array[] = {1, 2, 3, 4, 5};
	QString TxStr;
	QString ExpectedStr;
	unsigned int _tmp = EntryType_CNF;
	pCommDlg->fnConfigSetBlock((char*)&_test_array[0], 0x80, 5);
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	ExpectedStr = QString::fromLocal8Bit("TX: ");
//	ExpectedStr += QString::fromLocal8Bit("CONFIG : ");
	ExpectedStr += QString::fromLocal8Bit(CMD_SET_BLOCK);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 0x80;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(_test_array, 5);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	pCommDlg->crc32();
	_tmp = pCommDlg->ProcessCRC(reinterpret_cast<char*>(_test_array), 5);
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QLatin1Char('\r');
	ExpectedStr += QLatin1Char('\n');
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnConfigGetBlock(){
	unsigned char _test_array[5];
	QString TxStr;
	QString ExpectedStr;
	unsigned int _tmp = EntryType_CNF;
	pCommDlg->fnConfigGetBlock((char*)&_test_array[0], 0x80, 5);
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	ExpectedStr = QString::fromLocal8Bit("TX: ");
//	ExpectedStr += QString::fromLocal8Bit("CONFIG : 7F000001 : ");
	ExpectedStr += QString::fromLocal8Bit(CMD_GET_BLOCK);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 0x80;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 5;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QLatin1Char('\r');
	ExpectedStr += QLatin1Char('\n');
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnConfigStartWrite(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_CONFIG_START_WRITE);
	pCommDlg->fnConfigStartWrite();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}
//\========================
void Test_Communication::fnFwwareOn(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_FMWARE_ON);
	pCommDlg->fnFwwareOn();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnFwwareOff(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_FMWARE_OFF);
	pCommDlg->fnFwwareOff();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnFwwareSetBlock(){
	unsigned char _test_array[] = {1, 2, 3, 4, 5};
	QString TxStr;
	QString ExpectedStr;
	unsigned int _tmp = EntryType_FW;
	pCommDlg->fnFwwareSetBlock((char*)&_test_array[0], 0x80, 5);
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	ExpectedStr = QString::fromLocal8Bit("TX: ");
//	ExpectedStr += QString::fromLocal8Bit("CONFIG : 7F000001 : ");
	ExpectedStr += QString::fromLocal8Bit(CMD_SET_BLOCK);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 0x80;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(_test_array, 5);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	pCommDlg->crc32();
	_tmp = pCommDlg->ProcessCRC(reinterpret_cast<char*>(_test_array), 5);
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QLatin1Char('\r');
	ExpectedStr += QLatin1Char('\n');
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnFwwareGetBlock(){
	unsigned char _test_array[5];
	QString TxStr;
	QString ExpectedStr;
	unsigned int _tmp = EntryType_FW;
	pCommDlg->fnFwwareGetBlock((char*)&_test_array[0], 0x80, 5);
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	ExpectedStr = QString::fromLocal8Bit("TX: ");
//	ExpectedStr += QString::fromLocal8Bit("CONFIG : 7F000001 : ");
	ExpectedStr += QString::fromLocal8Bit(CMD_GET_BLOCK);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 1);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 0x80;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QString::fromLocal8Bit(" : ");
	_tmp = 5;
	_tmp = qFromBigEndian<quint32>(_tmp);
	ExpectedStr += pCommDlg->Data2Hex(reinterpret_cast<unsigned char*>(&_tmp), 4);
	ExpectedStr += QLatin1Char('\r');
	ExpectedStr += QLatin1Char('\n');
	QCOMPARE(TxStr, ExpectedStr); 
}

void Test_Communication::fnFwwareStartWrite(){
	QString TxStr;
	QString ExpectedStr = QString::fromLocal8Bit("TX: ");
	ExpectedStr += QString::fromLocal8Bit(CMD_FMWARE_START_WRITE);
	pCommDlg->fnFwwareStartWrite();
	emit sigCancel();
	TxStr = pCommDlg->getLogTxStr();
	QCOMPARE(TxStr, ExpectedStr); 
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
