/*!
 * @file debugviewdlg.h
 * @file debugview.cpp
 * @brief this file includes the Class for viewing debug messages 
 */
#pragma once
#include "include/typedefs.h"
//#include <QtCore\QSortFilterProxyModel.h>
//#include <QtGui\QStandardItemModel.h>
#include <QtGui>
//#include <QtCore\QStringList.h>
#include <QtCore>
#include <QtSerialPort/QtSerialPort>
#include "string.h"
#include "setupconnections_pch.h"
#include "setupconnections_global.h"
///port for communication with device
#define CONFIG_PORT			2020

/*! @enum CModeExchange 
*  @brief This enum describes mode communication with device  
*/
/*! @var CModeExchange::RS232Mode
*  Member 'RS232Mode' contains communication with device over serial port
*/
/*! @var CModeExchange::NetworkMode
*  Member 'NetworkMode' contains communication with device over ethernet
*/
typedef enum{
	RS232Mode = 0, NetworkMode
}CModeExchange;

IPS_UI(SetupConnectionsDialog)
/*! 
\defgroup setup_connections_module the class of the setup connections with device 
*/
class SETUPCONNECTIONS_DIALOG_SHARED_EXPORT SetupConnectionsDialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(SetupConnectionsDialog)

 public:
    using inherited = QDialog;
    using class_name = SetupConnectionsDialog;

 public:
/*!
\fn SetupConnectionsDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode)
\brief Function creation class
\param[in] parent pointer to the parent class
\param[in] mode communication mode with the device, default value - serial port
*/
    explicit SetupConnectionsDialog(QWidget* parent = nullptr, CModeExchange mode = CModeExchange::RS232Mode);
/*!
 \fn ~SetupConnectionsDialog()
 \brief Function destroy class
*/
    ~SetupConnectionsDialog() override;

 public:
/*!
 \fn instanceNetworkList()
 \brief Serial port / network device listing function
*/
    void instanceNetworkList();
/*!
 \fn insatnceEnable()
 \brief list settings function
*/
    void insatnceEnable();
/*!
\fn setConnectionsParamStr(QString _str)
\brief Communication parameter setting function
\param[in] _str Communication parameter
*/
	void setConnectionsParamStr(QString _str);
/*!
\fn getConnectionsParamStr()
\brief Communication parameter getting function
\return Communication parameter
*/
	QString getConnectionsParamStr();

 private:
/*!
\fn IP2Str_Dec( unsigned int _IP )
\brief Function convert IP-address to string
\param[in] _IP IP-address
\return IP-address in string-formatt
*/
	QString IP2Str_Dec( unsigned int _IP );

 private:
    Ui::SetupConnectionsDialog* ui = nullptr;
///mode communication with device
	CModeExchange modeConnections;
};
