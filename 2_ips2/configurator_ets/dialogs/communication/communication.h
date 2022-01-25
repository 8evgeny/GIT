/*!
\file communication.h
\brief This file contains a communication class with the device
*/

/*! 
\defgroup communication_module communication class with the device 
*/
#pragma once
#include "include/typedefs.h"
#include <QtCore>
//#include <QtCore\QTimer.h>
#include <QtSerialPort/QtSerialPort>
#include <QtEndian>
#include <QtNetwork>
#include <QtCore>
#include "../stringlist/string_list.h"
#include "../setupconnectionsdlg/setupconnections.h"
#include "../cryptodlg/cryptodlg.h"
#include "string.h"
#include "communication_pch.h"
#include "communication_global.h"

/*!
	Configuration block size for serial port
*/
#define CONFIG_BLOCK_SIZE_RS232		64
/*!
	Firmware block size for serial port
*/
#define FMWARE_BLOCK_SIZE_RS232		64
/*!
	Configuration block size for ethernet
*/
#define CONFIG_BLOCK_SIZE_ETH		512
/*!
	Firmware block size for ethernet
*/
#define FMWARE_BLOCK_SIZE_ETH		512
/*!
	Maximum size for configuration
*/
#define MAX_CONFIG_SIZE				1024*112
/*!
	Maximum size for firmware
*/
#define MAX_FMWARE_SIZE				1024*1024
/*!
	Identification system block data
*/
#define EntryType_SB		0
/*!
	Identification configuration block data
*/
#define EntryType_CNF		1
/*!
	Identification firmware block data
*/
#define EntryType_FW		2
/*!
	Identification phone book block data
*/
#define EntryType_TEL		3

/*!
	Pointer to empty block in EEPROM
*/
#define EEPROM_BLOCK_TYPE_UD	0
/*!
	Pointer to system block in EEPROM
*/
#define EEPROM_BLOCK_TYPE_SB	1
/*!
	Pointer to configuration block in EEPROM
*/
#define EEPROM_BLOCK_TYPE_CNF	2
/*!
	Pointer to firmware block in EEPROM
*/
#define EEPROM_BLOCK_TYPE_FW	3
/*!
	Pointer to phone book block in EEPROM
*/
#define EEPROM_BLOCK_TYPE_TEL	4

/*! @struct PartEntry 
*  @brief This structure describes the data block in EEPROM  
*/
/*! @var PartEntry::Type
*  Member 'Type' contains identification type block data 
*/
/*! @var PartEntry::Pos
*  Member 'Pos' contains the data block position in EEPROM
*/
/*! @var PartEntry::Size
*  Member 'Size' contains the data block size in EEPROM
*/
/*! @var PartEntry::CRC
*  Member 'CRC' contains the data block checksum in EEPROM
*/
/*! @var PartEntry::Ver
*  Member 'Ver' contains version in EEPROM
*/
struct  PartEntry{
	uint16_t Type;
	uint32_t Pos;
	uint32_t Size;
	uint32_t CRC;
	uint32_t Ver;
};
/*! @struct SystemBlockHdr 
*  @brief This structure describes system data block in EEPROM  
*/
/*! @var PartEntry::parts
*  Member 'parts' contains array the PartEntry blocks
*/
/*! @var PartEntry::KeyAES
*  Member 'KeyAES' contains array of the symbols AES-key
*/
/*! @var PartEntry::SN
*  Member 'SN' contains array of the symbols serial number device
*/
typedef struct{
	PartEntry parts[8];
	uint8_t  KeyAES[16];
	uint8_t  SN[12];
}SystemBlockHdr;

#define __IS_WRITE_LOG__

/*! \defgroup list_command_module Module contains list of commands communication with the device 
     \ingroup communication_module
*/

/*!
	Command request device type
*/
#define	CMD_GET_CFGUI				"DEVICE GET CFGUI\r\n"
/*!
	Command set data block
*/
#define	CMD_SET_BLOCK				"SET BLOCK"
/*!
	Command get data block
*/
#define CMD_GET_BLOCK				"GET BLOCK"
/*!
	Command initialization configure mode
*/
#define CMD_CONFIG_ON				"CONFIG ON\r\n"
/*!
	Command execute write configure data to EEPROM
*/
#define CMD_CONFIG_START_WRITE		"CONFIG START WRITE\r\n"
/*!
	Command deinitialization configure mode
*/
#define CMD_CONFIG_OFF				"CONFIG OFF\r\n"
/*!
	Command initialization firmware mode
*/
#define CMD_FMWARE_ON				"FMWARE ON\r\n"
/*!
	Command execute write firmware data to FLASH
*/
#define CMD_FMWARE_START_WRITE		"FMWARE START WRITE\r\n"
/*!
	Command deinitialization firmware mode
*/
#define CMD_FMWARE_OFF				"FMWARE OFF\r\n"

/*! \defgroup communication_dialog_module Class contains list of function, variables for communication with the device 
     \ingroup communication_module
*/
class COMMUNICATION_DIALOG_SHARED_EXPORT CommunicationDialog final: public QObject {
    Q_OBJECT
    IPS_DISABLE_COPY(CommunicationDialog)

 public:
    using inherited = QObject;
    using class_name = CommunicationDialog;

 public:
/*!
\fn CommunicationDialog(QWidget* parent = nullptr)
\brief Function creation class
\param[in] parent pointer to the parent class
*/
    explicit CommunicationDialog(QWidget* parent = nullptr);
/*!
 \fn ~CommunicationDialog()
 \brief Function destroy class
*/
	~CommunicationDialog() override;
public:
/*!
The function sets the communication mode with the device
\param[in] _mode communication mode with the device
*/
	void setMode(CModeExchange _mode);
/*!
\fn void setAesKey(uint8_t* key, uint8_t length_key)
\brief The function sets the encryption key 
\param[in] key array of symbols key
\param[in] length_key length of the key
*/
	void setAesKey(uint8_t* key, uint8_t length_key);
/*!
\fn void setSelfIP(QString _ip)
\brief The function sets self IP address for communication with the device 
\param[in] _ip self IP address
*/
	void setSelfIP(QString _ip);
/*!
\fn void setTargetIP(QString _ip)
\brief The function sets target IP address for communication with the device 
\param[in] _ip target IP address
*/
	void setTargetIP(QString _ip);
/*!
\fn void setRxUDPPort(uint32_t _port)
\brief The function sets udp port for receive message from device 
\param[in] _port udp port for receive message from device 
*/
	void setRxUDPPort(uint32_t _port);
/*!
 \fn bool Open()
 \brief The function open communication with the device 
 \return result open communication with the device 
*/
	bool Open();
/*!
 \fn void Close()
 \brief The function close communication with the device 
*/
	void Close();
/*!
 \fn void add2Log(QString _str, bool _isTx)
 \brief Function adds a string into a log
 \param[in] _str string to log
 \param[in] _isTx sign of rx/tx string with the device
*/
	void add2Log(QString _str, bool _isTx);
/*!
 \fn QString getLogTxStr()
 \brief The function returned current log string of the transfer into the device 
 \return current log string
*/
	QString getLogTxStr();
/*!
 \fn QString getLogRxStr()
 \brief The function returned current log string of the received from device 
 \return current log string
*/
	QString getLogRxStr();
/*!
\fn QString Data2Hex(unsigned char* _code, unsigned int _length_byte)
\brief Function convert data to array ASCII-symbols
\param[in] _code pointer to array of the data
\param[in] _length_byte length array of the data
\return array ASCII-symbols
*/
	QString Data2Hex(unsigned char* _code, unsigned int _length_byte);

/*!
\fn void setRs232PortName(QString portName)
\brief Function sets identification serial port
\param [in] portName	Serial port name
*/
	void setRs232PortName(QString portName);
/*!
\fn QString getLastErrorString()
\brief The function returns the string of the last error
\return  String of the last error
*/
	QString getLastErrorString();
/*!
\fn void crc32()
\brief The function initialize table checksum
*/
	void crc32();
/*!
 \fn unsigned int ProcessCRC(char* pData, unsigned int nLen)
 \brief Data block checksum calculation function
 \param [in] pData		Pointer to block data
 \param [in] nLen		Data block length
 \return               Checksum
*/
	unsigned int ProcessCRC(char* pData, unsigned int nLen);
/*!
 \fn bool fnGetCFGUI(unsigned int* _type)
 \brief Function reqest type of device
 \param [out] _type	Pointer into type device
 \return			Result executing command
*/
	bool fnGetCFGUI(unsigned int* _type);
/*!
 \fn bool fnConfigOn()
 \brief Function initialize configure mode of the device
 \return			Result executing command
*/
	bool fnConfigOn();
/*!
 \fn bool fnConfigOff()
 \brief Function deinitialize configure mode of the device
 \return			Result executing command
*/
	bool fnConfigOff();
/*!
 \fn bool fnConfigSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver)
 \brief The function sets system information of the configuration
 \param [in] _size	Size configure
 \param [in] _crc	Checksum configure
 \param [in] _ver	Version configure
 \return			Result executing command
*/
	bool fnConfigSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver);
/*!
  \fn bool fnConfigGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver)
  \brief The function get system information of the configuration
  \param [out] _size	Pointer into size configure
  \param [out] _crc		Pointer into checksum configure
  \param [out] _ver		Pointer into version configure
  \return				Result executing command
*/
	bool fnConfigGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver);
/*!
  \fn bool fnConfigSetDataSize(unsigned int _size)
  \brief The function set size of the configuration
  \param [in] _size	Size configure
  \return			Result executing command
*/
	bool fnConfigSetDataSize(unsigned int _size);
/*!
  \fn bool fnConfigGetDataSize(unsigned int* _data_size)
  \brief The function get size of the configuration
  \param [out] _data_size	Pointer into size configure
  \return					Result executing command
*/
	bool fnConfigGetDataSize(unsigned int* _data_size);
/*!
  \fn bool fnConfigSetBlock(char* _data, unsigned int _addr, unsigned int _size)
  \brief The function sets block data configuration
  \param [in] _data		Pointer into block data
  \param [in] _addr		Address in block data
  \param [in] _size		Length block data
  \return				Result executing command
*/
	bool fnConfigSetBlock(char* _data, unsigned int _addr, unsigned int _size);
/*!
  \fn bool fnConfigGetBlock(char* _data, unsigned int _addr, unsigned int _size)
  \brief The function gets block data configuration
  \param [out] _data		Pointer into block data
  \param [out] _addr		Address in block data
  \param [out] _size		Length block data
  \return				Result executing command
*/
	bool fnConfigGetBlock(char* _data, unsigned int _addr, unsigned int _size);
/*!
  \fn bool fnConfigSetCRC(unsigned int _crc)
  \brief The function sets checksum block data configuration
  \param [in] _crc	Checksum of the configuration
  \return			Result executing command
*/
	bool fnConfigSetCRC(unsigned int _crc);
/*!
  \fn bool fnConfigGetCRC(unsigned int* _crc)
  \brief The function gets checksum block data configuration
  \param [out] _crc	Pointer into checksum of the configuration
  \return			Result executing command
*/
	bool fnConfigGetCRC(unsigned int* _crc);
/*!
  \fn bool fnConfigStartWrite()
  \brief Function for initializing configuration writing to the EEPROM of the device
  \return			Result executing command
*/
	bool fnConfigStartWrite();
/*!
  \fn bool fnConfigSetVersion(int _version)
  \brief The function sets version configuration
  \param [in] _version	Version of the configuration
  \return			Result executing command
*/
	bool fnConfigSetVersion(int _version);
/*!
  \fn bool fnConfigGetVersion(int* _version)
  \brief The function gets version configuration
  \param [out] _version	Pointer into version of the configuration
  \return			Result executing command
*/
	bool fnConfigGetVersion(int* _version);
/*!
  \fn bool fnSetKeyAes(QString _key)
  \brief The function sets aes-key
  \param [in] _key	String of the aes-key
  \return			Result executing command
*/
	bool fnSetKeyAes(QString _key);
/*!
  \fn bool fnGetKeyAes(QString* _key)
  \brief The function gets aes-key
  \param [out] _key	Pointer to string of the aes-key
  \return			Result executing command
*/
	bool fnGetKeyAes(QString* _key);
/*!
  \fn bool fnSetSN(QString _sn)
  \brief The function sets serial number of the device
  \param [in] _sn	String of the serial number
  \return			Result executing command
*/
	bool fnSetSN(QString _sn);
/*!
  \fn bool fnGetSN(QString* _sn)
  \brief The function gets serial number of the device
  \param [out] _sn	Pointer into string of the serial number
  \return			Result executing command
*/
	bool fnGetSN(QString* _sn);
/*!
 \fn bool fnFwwareOn()
 \brief Function initialize firmware mode of the device
 \return			Result executing command
*/
	bool fnFwwareOn();
/*!
 \fn bool fnFwwareOff()
 \brief Function deinitialize firmware mode of the device
 \return			Result executing command
*/
	bool fnFwwareOff();
/*!
 \fn bool fnFwwareSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver)
 \brief The function sets system information of the firmware
 \param [in] _size	Size firmware
 \param [in] _crc	Checksum firmware
 \param [in] _ver	Version firmware
 \return			Result executing command
*/
	bool fnFwwareSetSysInfo(unsigned int _size, unsigned int _crc, unsigned int _ver);
/*!
  \fn bool fnFwwareGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver)
  \brief The function get system information of the firmware
  \param [out] _size	Pointer into size firmware
  \param [out] _crc		Pointer into checksum firmware
  \param [out] _ver		Pointer into version firmware
  \return				Result executing command
*/
	bool fnFwwareGetSysInfo(unsigned int* _size, unsigned int* _crc, unsigned int* _ver);
/*!
  \fn bool fnFwwareSetDataSize(unsigned int _size)
  \brief The function set size of the firmware
  \param [in] _size	Size firmware
  \return			Result executing command
*/
	bool fnFwwareSetDataSize(unsigned int _size);
/*!
  \fn bool fnFwwareGetDataSize(unsigned int* _data_size)
  \brief The function get size of the firmware
  \param [out] _data_size	Pointer into size firmware
  \return					Result executing command
*/
	bool fnFwwareGetDataSize(unsigned int* _data_size);
/*!
  \fn bool fnFwwareSetBlock(char* _data, unsigned int _addr, unsigned int _size)
  \brief The function sets block data firmware
  \param [in] _data		Pointer into block data
  \param [in] _addr		Address in block data
  \param [in] _size		Length block data
  \return				Result executing command
*/
	bool fnFwwareSetBlock(char* _data, unsigned int _addr, unsigned int _size);
/*!
  \fn bool fnFwwareGetBlock(char* _data, unsigned int _addr, unsigned int _size)
  \brief The function gets block data firmware
  \param [out] _data		Pointer into block data
  \param [out] _addr		Address in block data
  \param [out] _size		Length block data
  \return				Result executing command
*/
	bool fnFwwareGetBlock(char* _data, unsigned int _addr, unsigned int _size);
/*!
  \fn bool fnFwwareSetCRC(unsigned int _crc)
  \brief The function sets checksum block data firmware
  \param [in] _crc	Checksum of the firmware
  \return			Result executing command
*/
	bool fnFwwareSetCRC(unsigned int _crc);
/*!
  \fn bool fnFwwareGetCRC(unsigned int* _crc)
  \brief The function gets checksum block data configuration
  \param [out] _crc	Pointer into checksum of the configuration
  \return			Result executing command
*/
	bool fnFwwareGetCRC(unsigned int* _crc);
/*!
  \fn bool fnFwwareStartWrite()
  \brief Function for initializing firmware writing to the FLASH of the device
  \return			Result executing command
*/
	bool fnFwwareStartWrite();
/*!
  \fn bool fnFmwareSetVersion(int _version)
  \brief The function sets version firmware
  \param [in] _version	Version of the configuration
  \return			Result executing command
*/
	bool fnFmwareSetVersion(int _version);
/*!
  \fn bool fnFmwareGetVersion(int* _version)
  \brief The function gets version configuration
  \param [out] _version	Pointer into version of the configuration
  \return			Result executing command
*/
	bool fnFmwareGetVersion(int* _version);

public slots:
/*!
  \fn void serialPort_dataRead()
  \brief The function read data from serial port
*/
	 void serialPort_dataRead();
/*!
  \fn void pTimeTimeouts_timeout()
  \brief Function handler of the timeout waiting receive data from device
*/
	 void pTimeTimeouts_timeout();
/*!
  \fn void readPendingDatagrams()
  \brief The function read data from ethernet socket
*/
	 void readPendingDatagrams();
/*!
  \fn void cancel_transactions()
  \brief The function set cancel current translation command into device
*/
	 void cancel_transactions();
private:
/*!
  \fn void parse_msg(QString _msg)
  \brief The function parsing received answer from device
  \param [in] _msg	Message to parsing
*/
	void parse_msg(QString _msg);
/*!
  \fn bool setRqSetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size)
  \brief The function set block data into device
  \param [in] _type_block	Type of the block data
  \param [in] _addr			Addr into block data
  \param [in] _data			Pointer to block data
  \param [in] _size			Length block data
  \return					Result executing command
*/
	bool setRqSetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size);
/*!
  \fn bool setRqGetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size)
  \brief The function get block data into device
  \param [in] _type_block	Type of the block data
  \param [in] _addr			Addr into block data
  \param [out] _data		Pointer to block data
  \param [in] _size			Length block data
  \return					Result executing command
*/
	bool setRqGetBlock(unsigned char _type_block, unsigned int _addr, unsigned char* _data,  unsigned int _size);
/*!
  \fn void processTheDatagram(QByteArray dt)
  \brief The function handler data from ethernet socket
  \param [in] dt			Array data
*/
	void processTheDatagram(QByteArray dt);
private:
///Pointer to serial port component
	QSerialPort* serialPort;
///Pointer to udp socket;
	QUdpSocket* udp_socket;
///Pointer to timer of the timeout answer from device
	QTimer* pTimeTimeouts;
///Pointer to AES-module component
	CryptoDialog* AesModule;
///Flags timeouts answer from device
	bool isTimeoutAnswer;
///Flags of the receive answer from device
	bool isRcvStr;
///Flags state binding port udp socket
	bool OpenStateUdpSocket;
///Flags of the canceled current transaction
	bool isCancel;
///String received data from device
	QString rcvStr;
///String last error
	QString lastErrorStr;
///String of the name serial port
	QString PortName;
///String of the self IP address
	QString selfIP;
///String of the target IP address
	QString targetIP;
///Prepend section into message to device transfered for ethernet
	QString strPrepend;
///String full log message
	QString LogStr;
///String sender message
	QString LogTxStr;
///String received message
	QString LogRxStr;
///List of the strings received section in message
	QList<QString*> lstRcvAnswer;
///Mode exchange with the device
	CModeExchange modeExchange;
///Array of symbols AES-key
	uint8_t AesKey[32];
///Length AES-key
	uint8_t LengthAeskey;
///List receive messages
    QStringList rcvMsgList;
///Port receive messages
	uint32_t RxUDPPort;
};
