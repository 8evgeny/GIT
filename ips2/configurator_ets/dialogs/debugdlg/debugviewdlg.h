/*!
 * @file debugviewdlg.h
 * @file debugview.cpp
 * @brief this file includes the Class for viewing debug messages 
 */
#pragma once
#include "include/typedefs.h"
#include <QtCore\QStringList.h>
#include <QtCore\QSaveFile.h>
#include <QtCore\QFile.h>
#include <QtSerialPort/QtSerialPort>
#include <QtNetwork>
#include "../stringlist/string_list.h"
#include "../cryptodlg/cryptodlg.h"
#include "../setupconnectionsdlg/setupconnections.h"
#include "../msgitemsmodel/msgtablemodel.h"
#include "string.h"
#include "debugviewdlg_pch.h"
#include "debugviewdlg_global.h"
///mode enable command 
#define CMD_DEBUG_ON				"DEBUG ON\r\n"
///mode disable command
#define CMD_DEBUG_OFF				"DEBUG OFF\r\n"

IPS_UI(DebugViewDialog)
/*! 
\defgroup debugview_module the class of the configuration device 
*/
class DEBUGVIEW_DIALOG_SHARED_EXPORT DebugViewDialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(DebugViewDialog)

 public:
    using inherited = QDialog;
    using class_name = DebugViewDialog;

 public:
/*!
\fn DebugViewDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode)
\brief Function creation class
\param[in] parent pointer to the parent class
\param[in] mode communication mode with the device, default value - serial port
*/
    explicit DebugViewDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode);
/*!
 \fn ~DebugViewDialog()
 \brief Function destroy class
*/
    ~DebugViewDialog() override;

 public:
/*!
 \fn setTitle(QString _title)
 \brief window title setting function
 \param[in] _title Window title
*/
    void setTitle(QString _title);
/*!
 \fn setMode(CModeExchange _mode)
 \brief mode communication setting function
 \param[in] _mode communication mode
*/
	void setMode(CModeExchange _mode);
/*!
 \fn setRS232Name(QString _portname)
 \brief port communication name setting function
 \param[in] _mode port communication name
*/
	void setRS232Name(QString _portname);
/*!
 \fn setSelfIP(QString _selfip)
 \brief self IP address setting function
 \param[in] _selfip self IP address
*/
	void setSelfIP(QString _selfip);
/*!
 \fn setAesKey(uint8_t* key, uint8_t length_key)
 \brief encryption key setting function
 \param[in] key pointer to encryption key
 \param[in] key length encryption key
*/
	void setAesKey(uint8_t* key, uint8_t length_key);
/*!
 \fn setConfigureStr(QString _data)
 \brief config string setting function
 \param[in] _data config string
*/
	void setConfigureStr(QString _data);
/*!
 \fn appendMsg(QString _dt, QString _cid, QString _ip, QString _msg)
 \brief Add message function 
 \param[in] _dt current date time
 \param[in] _cid cid station
 \param[in] _ip IP-address station
 \param[in] _msg current message
*/
	void appendMsg(QString _dt, QString _cid, QString _ip, QString _msg);
/*!
\fn QString Data2Hex(unsigned char* _code, unsigned int _length_byte)
\brief Function convert data to array ASCII-symbols
\param[in] _code pointer to array of the data
\param[in] _length_byte length array of the data
\return array ASCII-symbols
*/
	QString Data2Hex(unsigned char* _code, unsigned int _length_byte);
/*!
 \fn fnDebugOn()
 \brief mode enable function
*/
	void fnDebugOn();
/*!
 \fn fnDebugOff()
 \brief mode disable function
*/
	void fnDebugOff();

 public slots:
/*!
 \fn b_start_clicked()
 \brief debug mode enable button handler 
*/
	 void b_start_clicked();
/*!
 \fn b_savelog_clicked()
 \brief save list message button handler 
*/
	 void b_savelog_clicked();
/*!
 \fn b_loadlog_clicked()
 \brief load list message button handler 
*/
	 void b_loadlog_clicked();
/*!
 \fn cb_liststations_currentIndexChanged(int index)
 \brief station index selection handler 
 \param[in] index identificator station
*/
	 void cb_liststations_currentIndexChanged(int index);
/*!
 \fn serialPort_dataRead()
 \brief handler for receiving data from serial port
*/
	 void serialPort_dataRead();
/*!
 \fn pTimeTimeouts_timeout()
 \brief response timer handler
*/
	 void pTimeTimeouts_timeout();
/*!
 \fn readPendingDatagrams()
 \brief handler for receiving data from ethernet
*/
	 void readPendingDatagrams();

	 void testTimer_timeout();

 private:
/*!
 \fn getIPFromCid(QJsonArray* StationList, QString _cid)
 \brief function of searching the IP-address of the station by cid 
 \param[in] StationList pointer to array of station list describes
 \param[in] _cid identification station
 \return IP-address
*/
	QString getIPFromCid(QJsonArray* StationList, QString _cid);
/*!
 \fn processTheDatagram(QByteArray dt)
 \brief handler receive datagramm
 \param[in] dt current datagramm
*/
	void processTheDatagram(QByteArray dt);

 private:
    Ui::DebugViewDialog* ui = nullptr;
///list of station cids
	QStringList* pCidList;
///configure plan
	QString globalConfigureStr;
///serial port name
	QString portName;
///Self IP-address
	QString selfIP;
///target IP-Address
	QString targetIP;
///mode communication with device
	CModeExchange ModeExchange;
///pointer to model debug view messages
	MsgTableModel* model; 
///flag of start mode
	bool isStart;
///pointer to serial port component
	QSerialPort* serialPort;
///pointer to udp socket
	QUdpSocket* udp_socket;
	QTimer* pTimeTimeouts;
///flag state udp socket
	bool OpenStateUdpSocket;
///flag timeout answer from device
	bool isTimeoutAnswer;
///flag receive answer from device
	bool isRcvStr;
///receive message
	QString rcvStr;
///string prepend to transmitt data over ethernet
	QString strPrepend;
///encryption key buffer
	uint8_t AesKey[32];
///length of the encryption key
	uint8_t LengthAeskey;
///pointer to encryption module
	CryptoDialog* AesModule;
///Port receive messages
	unsigned int RxUDPPort;

	QTimer* testTimer;
};
