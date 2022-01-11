#ifndef MDRUARTLOADER_H
#define MDRUARTLOADER_H

#include "mdrmcudescriptor.h"

#include <QObject>
#include <QSerialPort>

class MdrUartLoaderClient : public QObject
{
	Q_OBJECT
	
	enum {
		SYNC_ATTEMPT_COUNT = 2000,
		SYNC_SYNC_BLOCK_SIZE = 20,
	};

	public:

	enum Command {
		CMD_SYNC = 0x00, // ноль- символ синхронизаци скорости обмена.
		CMD_CR   = 0x0D, //     - пустая команда - возвращает 0d 0a 3e.
//		CMD_LF   = 0x0A, //      !не команда мастеру!
//		CMD_SOK  = 0x3E, // '>'  !не команда мастеру! - sync ok.
		CMD_BAUD = 0x42, // 'B' - установка скорости обмена.
		CMD_LOAD = 0x4C, // 'L' - загрузка массива байт.
		CMD_VFY  = 0x59, // 'Y' - чтение (выдача) массива байт.
		CMD_RUN  = 0x52  // 'R' - запуск программы на выполнение.
	};

	enum Error {
		REPLY_OK = 0x4B, // 'K' - успешное звершение выполнения команды.
		// код комманды состоит из двух символов: первый 0x45 ('E') - признак ошибки
		ERR_FLAG = 0x45, // 'E'
		// второй символ - определяет тип ошибки.
		ERR_CHN  = 0x69, // 'i' - аппаратная ошибка UART.
		ERR_CMD  = 0x63, // 'c' - принята неизвестная команда.
		ERR_BAUD = 0x62  // 'b' - недопустиая скорость обмена.
	};

	enum ErrorCode {
		NoError,
		CanNotOpenPort,
		PortIsNotOpen,
		McuDoesNotAnswer,
		LoaderHwError,
		LoaderUnknownCmd,
		LoaderUnknownErr,
		InvalidParametr,
		DataTransferError,
		ProtocolError,
		
		UnsupportedMcuType,
		UndefinedMcuRevision,
		UndefinedCM1,
		UndefinedCM3,
		
	};
	
	enum {
		// В качестве значения параметра запрещено использовать число 0xFFFFFFFF.
		FORBIDDEN_PARAMETER_VALUE = 0xFFFFFFFFU,
	};
	
	explicit MdrUartLoaderClient(//QString portName = QString(),
	                             QObject *parent = nullptr);
	~MdrUartLoaderClient();
	int openPort(QString portName = QString(),
	             qint32 baudRate = QSerialPort::Baud9600);
	void closePort();

	int ping();
	int sync(void);
	int setSpeed(int baudrate);
	int tryConnect();
	
	int read(uint32_t addr, uint32_t size, QByteArray &data);
	int readWordA(uint32_t addr, uint32_t& word);
	int getArmCpuid(ARM::CPUID_t &cpuid);
	int getMdrMcuTypeAndRev(Mcu::Type& mcuType, int& rev);
	int getBootROM(Mcu::Type mcuType, QByteArray &data);
	int getFlash(Mcu::Type mcuType, QByteArray &data);
	
	int send(uint32_t addr, const QByteArray& data);
	int writeAndVerify(uint32_t addr, const QByteArray& data);
	
	int run(uint32_t addr);
	
	QString portErrorString() const { return port->errorString(); }
	//QSerialPort::SerialPortError portError() const { return port->error(); }
	//QSerialPort *serialPort() const { return port; }
	
	static const QString ecToString(int ec);
	
	private:
	QSerialPort *port;

	int getVE1VE3flash(QByteArray &data);
	QByteArray portWR(const QByteArray& send, int expectedResponseBytesCount, int timeout);
};

#endif // MDRUARTLOADER_H
