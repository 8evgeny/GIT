/*!
 * @file configuredlg.h
 * @file configuredlg.cpp
 * @brief This file includes the class of the configuration device 
 */
#pragma once
#include "include/typedefs.h"
#include <QtCore\QStringList.h>
#include <QtCore\QSaveFile.h>
#include <QtCore\QFile.h>
#include <QtCore\QTimer.h>
#include <QtCore\QJsonDocument>
#include <QtCore\QJsonArray>
#include <QtCore\QJsonObject>
#include <QtWidgets\QProgressBar>
#include <QtWidgets\QProgressDialog.h>
#include <QtSerialPort/QtSerialPort>
#include "../stringlist/string_list.h"
#include "../setupconnectionsdlg/setupconnections.h"
#include "../communication/communication.h"
#include "../../main/classes/base_types/station_types.h"
#include "string.h"
#include "configuredlg_pch.h"
#include "configuredlg_global.h"

IPS_UI(ConfigureDialog)

/*! 
\defgroup configure_module the class of the configuration device 
*/
class CONFIGURE_DIALOG_SHARED_EXPORT ConfigureDialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(ConfigureDialog)

 public:
    using inherited = QDialog;
    using class_name = ConfigureDialog;

 public:
/*!
\fn ConfigureDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode)
\brief Function creation class
\param[in] parent pointer to the parent class
\param[in] mode communication mode with the device, default value - serial port
*/
    explicit ConfigureDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode);
/*!
 \fn ~ConfigureDialog()
 \brief Function destroy class
*/
    ~ConfigureDialog() override;

signals:
/*!
 \fn sigCancel()
 \brief a function that emits a signal to cancel the current transaction 
*/
	void sigCancel();

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
 \brief function self IP address setting function
 \param[in] _selfip self IP address
*/
	void setSelfIP(QString _selfip);
/*!
 \fn setConfigureStr(QString _data)
 \brief config string setting function
 \param[in] _data config string
*/
	void setConfigureStr(QString _data);
/*!
 \fn getConfigureStr()
 \brief config string getting function
 \return config string
*/
	QString getConfigureStr();
/*!
 \fn isAcceptLoaddedConfigure()
 \brief new configuration confirmation function 
 \return confirmation flags
*/
	bool isAcceptLoaddedConfigure();
/*!
 \fn getLN_SN()
 \brief the function returns a pointer to the serial number input string 
 \return pointer to the serial number input string 
*/
	QLineEdit* getLN_SN();
/*!
 \fn getLN_SN()
 \brief the function returns a pointer to the encryption key input string 
 \return pointer to the encryption key input string  
*/
	QLineEdit* getLN_KeyAes();
/*!
 \fn getLN_m_settings()
 \brief the function returns a pointer to the selected cid input string 
 \return pointer to the selected cid input string  
*/
	QLineEdit* getLN_m_settings();
/*!
 \fn getLN_IPStation()
 \brief the function returns a pointer to the selected IP address input string 
 \return pointer to the selected IP address input string  
*/
	QLineEdit* getLN_IPStation();
/*!
 \fn getBtn_b_setSN()
 \brief the function returns the pointer to the button for setting the serial number of the product 
 \return pointer to the serial number buttons  
*/
	QToolButton* getBtn_b_setSN();
/*!
 \fn getBtn_b_GetAES()
 \brief the function returns the pointer to the button for getting the product encryption key 
 \return pointer to the button for getting the product encryption key  
*/
	QToolButton* getBtn_b_GetAES();
/*!
 \fn getBtn_b_SetAES()
 \brief the function returns the pointer to the button for setting the product encryption key  
 \return pointer to the button for setting the product encryption key  
*/
	QToolButton* getBtn_b_SetAES();
/*!
 \fn getBtn_b_send()
 \brief the function returns the pointer to the product configuration button 
 \return pointer to the product configuration button
*/
	QPushButton* getBtn_b_send();
/*!
 \fn getBtn_b_rcv()
 \brief the function returns a pointer to the product configuration read button
 \return pointer to the product configuration read button
*/
	QPushButton* getBtn_b_rcv();
/*!
 \fn setStateTestingMode(bool _isTestMode)
 \brief test mode set function
 \param[in] _isTestMode test mode
*/
	void setStateTestingMode(bool _isTestMode);
/*!
 \fn isTestResult()
 \brief the function returns the test result 
 \return test result
*/
	bool isTestResult();
/*!
 \fn getTestMsg()
 \brief the function returns the test result message
 \return test result message
*/
	QString getTestMsg();

 public slots:
/*!
 \fn b_open_clicked()
 \brief the function selection cid device
*/
	 void b_open_clicked();
/*!
 \fn b_send_clicked()
 \brief product configuration recording function
*/
	 void b_send_clicked();
/*!
 \fn b_rcv_clicked()
 \brief product configuration read function
*/
	 void b_rcv_clicked();
/*!
 \fn b_SetSN_clicked()
 \brief function of setting the serial number of the product
*/
	 void b_SetSN_clicked();
/*!
 \fn b_GetAES_clicked()
 \brief product encryption key getting function
*/
	 void b_GetAES_clicked();
/*!
 \fn b_SetAES_clicked()
 \brief product encryption key setting function
*/
	 void b_SetAES_clicked();
/*!
 \fn pdSendConfig_cancel()
 \brief function to undo the current transaction
*/
	 void pdSendConfig_cancel();

 protected:
/*!
 \fn closeEvent( QCloseEvent *ev)
 \brief window close event function
 \param[in] ev pointer to window close event
*/
	void closeEvent( QCloseEvent *ev);

 private:
/*!
 \fn ConvertToConfigureStr(QString _cid)
 \brief function of converting a configuration from a communication plan to a configuration for a selected device 
 \param[in] _cid identification station string
 \return Device configuration in json format  
*/
	QString ConvertToConfigureStr(QString _cid);
/*!
 \fn getIPFromCid(QJsonArray* StationList, QString _cid)
 \brief function of finding the IP-address of the station by cid
 \param[in] StationList list of the station
 \param[in] _cid identification station string
 \return IP address  
*/
	QString getIPFromCid(QJsonArray* StationList, QString _cid);
/*!
 \fn addGroupCallForGid(QJsonArray* GroupCallList, QString _gid)
 \brief function of adding information about the used call group
 \param[out] GroupCallList pointer to group call list
 \param[in] _cid identification station string
*/
	void addGroupCallForGid(QJsonArray* GroupCallList, QString _gid);
/*!
 \fn getStationInfoFromHash(QJsonArray* _dstList, QJsonArray* pStationArray, QString _hash)
 \brief function of finding information about the cid and ip of the station by hash-value 
 \param[out] _dstList pointer to information about the cid and ip of the station
 \param[in] pStationArray pointer to array of station description
 \param[in] _hash hash-value station string
*/
	void getStationInfoFromHash(QJsonArray* _dstList, QJsonArray* pStationArray, QString _hash);
/*!
 \fn checkGidInGroupCallList(QJsonArray* _groupCallList, int _gid)
 \brief function for checking the presence of information in the array of group calls about the group gid  
 \param[in] _groupCallList pointer to array of call groups 
 \param[in] _gid group identification string
*/
	bool checkGidInGroupCallList(QJsonArray* _groupCallList, int _gid);
/*!
 \fn updateSNInRcvConfig(QString _sn)
 \brief product serial number update function
 \param[in] _sn new serial number device 
*/
	void updateSNInRcvConfig(QString _sn);

 private:
    Ui::ConfigureDialog* ui = nullptr;
///station identifier list 
	QStringList* pCidList;
///filtered list of station identifiers
	QStringList filteredCidList;
///station identifier selection dialog form
	StringListDialog* dlgCidList;
///instance of the object of the module for transmitting the configuration to the station 
	CommunicationDialog* pCommunicationDialog;
///an instance of the display module of the configuration transfer process 
	QProgressDialog* pdSendConfig;
///configuration byte array to transfer
	QByteArray baTxConfiguration;
///sign of cancellation of the configuration process
	bool isCancelWrite;
///buffer for transmission 
	char test_buff_tx[1024*1024];
///receive buffer
	char test_buff_rx[1024*1024];
///size of the received configuration
	unsigned int rxSizeConfig;
///checksum of the received configuration
	unsigned int rxCRCConfig;
///size of the transferred configuration
	int txSizeConfig;
///checksum of the transferred configuration
	unsigned int txCRCConfig;
///configuration plan in json-format
	QString globalConfigureStr;
///received configuration in json-format
	QString rcvConfigStr;
///configuration for device in json-format
	QString sendConfigStr;
///type of the device
	int typeDeviceConfiguring;
///identification of the device
	QJsonObject cnfDevice;
///flag of receiving configuration from device
	bool isRcvConfigure;
///configuration progress indicator  
	QProgressBar* pb;
///button <Cancel>
	QPushButton* btnCancel;
///serial port name
	QString portName;
/// self IP address
	QString selfIP;
///IP address of the device
	QString targetIP;
///Encryption key from communication plan
	QString encKey;
///communication mode
	CModeExchange ModeExchange;
///configuration mode
	int version;
///test mode flag 
	bool isTestMode;
///test mode result flag
	bool testResult;
///test message string
	QString testMsg;
///Port receive messages
	unsigned int RxUDPPort;
};
