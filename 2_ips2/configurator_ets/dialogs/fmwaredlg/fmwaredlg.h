/*!
 * @file fmwaredlg.h
 * @file fmwaredlg.cpp
 * @brief This file includes the class of the firmware device 
 */
#pragma once
#include "include/typedefs.h"
#include <QtCore\QStringList.h>
#include <QtCore\QSaveFile.h>
#include <QtCore\QFile.h>
#include <QtCore\QDir>
#include <QtCore\QSettings>
#include <QtCore\QTimer.h>
#include <QtCore\QJsonDocument>
#include <QtCore\QJsonArray>
#include <QtCore\QJsonObject>
#include <QtWidgets\QProgressBar>
#include <QtWidgets\QProgressDialog>
#include <QtWidgets\QRadioButton>
#include <QtWidgets\QCheckBox>
#include <QtWidgets\QFileDialog>
#include <QCloseEvent>
#include "../stringlist/string_list.h"
#include "../setupconnectionsdlg/setupconnections.h"
#include "../communication/communication.h"
#include "../../gt/settings/settings.h"
#include "string.h"
#include "fmwaredlg_pch.h"
#include "fmwaredlg_global.h"

IPS_UI(FmWareDialog)

/*! 
\defgroup firmware_module the class of the firmware device 
*/
class FMWARE_DIALOG_SHARED_EXPORT FmWareDialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(FmWareDialog)

 public:
    using inherited = QDialog;
    using class_name = FmWareDialog;

 public:
/*!
\fn FmWareDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode)
\brief Function creation class
\param[in] parent pointer to the parent class
\param[in] mode communication mode with the device, default value - serial port
*/
    explicit FmWareDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode);
/*!
 \fn ~ConfigureDialog()
 \brief Function destroy class
*/
    ~FmWareDialog() override;

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
 \brief self IP address setting function
 \param[in] _selfip self IP address
*/
	void setSelfIP(QString _selfip);
/*!
 \fn setConfigureStr(QString _data)
 \brief config string setting function
 \param[in] _data config string
*/
	void setConfigureStr(QString _data);

public:
/*!
 \fn get_b_close()
 \brief the function returns the pointer to the button for close window 
 \return pointer to close window buttons  
*/
	QPushButton* get_b_close();
/*!
 \fn get_b_loadFirmFile()
 \brief The function returns a pointer to the button for reading the firmware file 
 \return pointer to the button for reading the firmware file  
*/
	QPushButton* get_b_loadFirmFile();
/*!
 \fn get_b_ReadVersions()
 \brief The function returns a pointer to the button for reading the version firmware device 
 \return pointer to the button for reading the version firmware device  
*/
	QPushButton* get_b_ReadVersions();
/*!
 \fn get_b_send()
 \brief The function returns a pointer to the send firmware button  
 \return pointer to the send firmware button  
*/
	QPushButton* get_b_send();
/*!
 \fn get_b_rcv()
 \brief The function returns a pointer to the button for receiving the firmware
 \return pointer to the button for receiving the firmware
*/
	QPushButton* get_b_rcv();
/*!
 \fn get_lineEdit_KeyAES()
 \brief The function returns a pointer to the encryption key input string 
 \return pointer to the encryption key input string
*/
	QLineEdit*	 get_lineEdit_KeyAES();
/*!
 \fn get_lineEdit_version()
 \brief The function returns a pointer to the version firmware input string 
 \return pointer to the version firmware input string
*/
	QLineEdit*	 get_lineEdit_version();
/*!
 \fn get_comboBox_TypeDev()
 \brief The function returns a pointer to the drop-down list of device types 
 \return pointer to the drop-down list of device types
*/
	QComboBox*	 get_comboBox_TypeDev();
/*!
 \fn get_checkBox_all()
 \brief The function returns a pointer to the selector for selecting all devices in the list  
 \return pointer to the selector for selecting all devices in the list
*/
	QCheckBox*	 get_checkBox_all();
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
///test mode flag
	bool isTestMode;
///test mode result flag
	bool testResult;
///test message string
	QString testMsg;

 public slots:
/*!
 \fn b_send_clicked()
 \brief product firmware recording function
*/
	 void b_send_clicked();
/*!
 \fn b_rcv_clicked()
 \brief firmware read function
*/
	 void b_rcv_clicked();
/*!
 \fn pdSendConfig_cancel()
 \brief Button handling function Cancel
*/
	 void pdSendConfig_cancel();
/*!
 \fn comboBox_TypeDev_currentIndexChanged(int _idx)
 \brief Function for selecting index device type 
 \param[in] _idx	index device type
*/
	 void comboBox_TypeDev_currentIndexChanged(int _idx);
/*!
 \fn checkBox_all_stateChanged(int _state)
 \brief Function to select all devices in the list  
 \param[in] _state	to select all devices in the list
*/
	 void checkBox_all_stateChanged(int _state);
/*!
 \fn b_loadFirmFile_clicked()
 \brief Firmware file selection function 
*/
	 void b_loadFirmFile_clicked();
/*!
 \fn b_ReadVersions_clicked()
 \brief Firmware version reading function 
*/
	 void b_ReadVersions_clicked();

protected:
/*!
 \fn closeEvent( QCloseEvent *ev)
 \brief window close event function
 \param[in] ev pointer to window close event
*/
	void closeEvent( QCloseEvent *ev);

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
 \fn getTypeFromCid(QJsonArray* StationList, QString _cid)
 \brief function of searching the type of the device by cid 
 \param[in] StationList pointer to array of station list describes
 \param[in] _cid identification station
 \return type of the device
*/
	int getTypeFromCid(QJsonArray* StationList, QString _cid);
/*!
 \fn updateListCID()
 \brief function update list identification station 
*/
	void updateListCID();
/*!
 \fn drawCidStatus(QString _selected_cid, bool _isOkStatus)
 \brief function for drawing the result of the action
 \param[in] _selected_cid identification station
 \param[in] _isOkStatus status result of the action
*/
	void drawCidStatus(QString _selected_cid, bool _isOkStatus);

 private:
    Ui::FmWareDialog* ui = nullptr;
///filtered list of identifiers 
	QStringList filteredCidList;
///list of identifiers 
	QStringList listCids;
///pointer to communication dialog module
	CommunicationDialog* pCommunicationDialog;
///pointer to form progress dialog of the action
	QProgressDialog* pdSendConfig;
///pointer to progress bar
	QProgressBar* pb;
//	QList<QToolButton*> listStatus;
///list of the pointer to multi-selector station
	QList<QCheckBox*> listCB;
///list of the pointer to one-selector station
	QList<QRadioButton*> listRB;
///flag cancel of the action
	bool isCancelWrite;
///window closing prohibit flag 
	bool isDisableCloseWindow;
//	int typeDeviceConfiguring;
///buffer to transmitt firmware
	char buff_tx[1024*1024];
///checksum firmware
	unsigned int txCRCFmware;
///version firmware
	int version;
	QJsonObject cnfDevice;
///configuration plan string
	QString globalConfigureStr;
///RS232 port name
	QString portName;
///Self IP-address
	QString selfIP;
///mode communication with device
	CModeExchange ModeExchange;
///firmware filename
	QString fmware_filename;
//pointer to button Cancel
	QPushButton* btnCancel;
///Port receive messages
	unsigned int RxUDPPort;
	QSettings pSettings;
};
