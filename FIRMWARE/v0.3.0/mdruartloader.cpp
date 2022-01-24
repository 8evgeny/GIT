/*
 * 
 * */

#include "string.h"

#include <QDebug>
#include <QElapsedTimer>

#include "bytearray.h"
#include "mdruartloader.h"

static const QByteArray invite("\x0d\x0a\x3e", 3);

MdrUartLoaderClient::MdrUartLoaderClient(/*QString portName,*/ QObject *parent)
  : QObject(parent)
  , port(new QSerialPort)
{
	port->setBaudRate(QSerialPort::BaudRate::Baud9600);
	port->setDataBits(QSerialPort::DataBits::Data8);
	port->setStopBits(QSerialPort::StopBits::OneStop);
	port->setParity  (QSerialPort::Parity::NoParity);
	//port->setPortName(portName);
}

MdrUartLoaderClient::~MdrUartLoaderClient()
{
	//closePort();
	delete port;
}

int // error code
MdrUartLoaderClient::openPort(QString portName, qint32 baudRate)
{
	if(port->portName() != portName || port->baudRate() != baudRate) {
		if(portName.isEmpty()) return InvalidParametr;
		port->close();
		port->setPortName(portName);
		port->setBaudRate(baudRate);
	}
	if( !port->isOpen()) {
		if( !port->open(QIODevice::OpenModeFlag::ReadWrite)) {
			qDebug()<<port->error()<<port->errorString();
			return CanNotOpenPort;
		}
	}
	return NoError;
}

void
MdrUartLoaderClient::closePort()
{
	port->close();
}


int // error code
MdrUartLoaderClient::ping()
{
	if( !port->isOpen()) return PortIsNotOpen;
	port->clear();
	
	QByteArray req(1, char(CMD_CR));
	QByteArray ans = portWR(req, 3, 250);
	if(ans.startsWith(invite)) {
		return NoError; // есть ответ.
	}
	qDebug("ping err: req[%d] '%s', ans[%d] '%s'.",
	       req.size(), req.toHex().constData(),
	       ans.size(), ans.toHex().constData());
	return McuDoesNotAnswer;
}

int // error code
MdrUartLoaderClient::sync()
{
	if( !port->isOpen()) return PortIsNotOpen;
	port->clear();
	
	QByteArray ans, req(1, char(CMD_SYNC));
	req = req.repeated(SYNC_SYNC_BLOCK_SIZE);
	// NOTE если МК уже синхронизирован - он ответит на CMD_CR.
	// NOTE Т.е. сперва стоит проверить наличие связи пингом.
	int attempt = SYNC_ATTEMPT_COUNT / SYNC_SYNC_BLOCK_SIZE;
	do {
		port->write(req);
		port->waitForReadyRead(SYNC_SYNC_BLOCK_SIZE); // примерно соответствует
		ans += port->readAll();
		if(ans.startsWith(invite)) {
			qDebug("sync OK: send %d packets by %d chars.",
			       SYNC_ATTEMPT_COUNT / SYNC_SYNC_BLOCK_SIZE - attempt,
			       SYNC_SYNC_BLOCK_SIZE);
			return NoError; // есть ответ.
		}
		if(ans.length()>1) {
			if(ans.at(0) == ERR_FLAG) {
				switch(ans.at(1)) {
				case ERR_CHN: return LoaderHwError;
				case ERR_CMD: return LoaderUnknownCmd;
				default: return LoaderUnknownErr;
				}
			} else {
				if(ans.length()>invite.length()) ans.remove(0, 1);
			}
		}
	} while( --attempt /*|| !hasAnswer*/ );
	qDebug("sync err: req[%d] '%s', ans[%d] '%s'.",
	       req.size(), req.toHex().constData(),
	       ans.size(), ans.toHex().constData());
	return McuDoesNotAnswer;
}

int MdrUartLoaderClient::setSpeed(int32_t baudrate)
{
	if( !port->isOpen()) return PortIsNotOpen;
	if(static_cast<unsigned>(baudrate) == FORBIDDEN_PARAMETER_VALUE
	   || baudrate<0 ) return InvalidParametr;
	
	ByteArray req(1, char(CMD_BAUD));
	req.append(baudrate);
	qDebug("setSpeed: baudrate %d, req[%d] '%s'.",
	       baudrate, req.length(), req.toHex().constData());
	
	port->clear();
	port->write(req);
	port->waitForReadyRead(50);
	closePort();
	return openPort(port->portName(), baudrate);
}

int // error code
MdrUartLoaderClient::tryConnect()
{
	int ec = ping();
	if(ec == McuDoesNotAnswer) {
		qDebug("tryConnect: sync()...");
		ec = sync();
	} else {
		qDebug("tryConnect: already synced.");
	}
	return ec;
}

/**
 * @brief MdrUartLoaderClient::read Побайтовое чтение массива данных 
 *                                  по произвольному адресу из памяти МК.
 * @param [in] addr
 * @param [in] size
 * @param [out] data
 * @return ErrorCode
 */
int // error code
MdrUartLoaderClient::read(uint32_t addr, uint32_t size, QByteArray &data)
{
	if( !port->isOpen()) return PortIsNotOpen;
	port->clear();
	
	qDebug("read: addr 0x%X, size %d.", addr, size);
	if(addr==FORBIDDEN_PARAMETER_VALUE) return InvalidParametr;
	if(size==FORBIDDEN_PARAMETER_VALUE) return InvalidParametr;
	data.clear();
	
	ByteArray req(1, char(CMD_VFY));
	req.append(addr);
	req.append(size);
	//qDebug()<<"read: raw header: size"<<req.size()<<req.toHex();
	port->clear();
	port->write(req);
	// тут была какая-то сложная мысль. См. проект mdrwalker версии от июня 2021.
	int len = int(size) + 2;
	QByteArray rx;
	rx.reserve(len);
	
	QElapsedTimer t;
	t.start();
	qint64 to_ms = 300; // time out, [мс]
	qint64 tl_ms = len; // time limit, [ms]
	tl_ms =(tl_ms *  1100 * 10 )/ port->baudRate() + to_ms;
	//   кол.байт милли+рез бит   скорость[бит/с]   запас
	do {
		port->waitForReadyRead(100);
		if(0 == port->bytesAvailable()) {
			qDebug("read: recieving data particle: %d of %d", rx.size()-1, size);
			//break; //return -9;
		}
		rx.append(port->readAll());
		if(rx.size() == 0 && t.elapsed() > to_ms) {
			break; // данных нет уже to_ms - выходим.
		}
	} while (rx.size() < len && t.elapsed() < tl_ms);
	
	qDebug("read: time elapsed %lld ms, timeout was %lld ms", t.elapsed(), tl_ms);
	
	if(rx.size() < len) {
		qDebug("read: recieved %d bytes, expected %d.", rx.size(), len);
		return DataTransferError;
	}
	if(rx.at(0) != CMD_VFY) {
		qDebug("read: V (cmd_vfy) not found - '%c'.", rx.at(0));
		return ProtocolError;
	}
	if(rx.at(len-1) != REPLY_OK) {
		qDebug("read: K (reply_ok) not found - '%c'.", rx.at(len-1));
		return ProtocolError;
	}
	data = rx.mid(1, static_cast<int>(size));
	return NoError;
}

/**
 * @brief MdrUartLoaderClient::readWordA Чтение 32-битного слова данных.
 * @param [in] addr
 * @param [out] word
 * @return ErrorCode
 * @warning Чтение осуществляется побайтово!
 */
int // error code
MdrUartLoaderClient::readWordA(uint32_t addr, uint32_t &word)
{
	QByteArray data;
	int ec = read(addr, 4, data);
	if(ec) {
		word = 0;
		return ec;
	}
	memcpy(&word, data.constData(), 4);
	return NoError;
}

/**
 * @brief MdrUartLoaderClient::getBootROM
 * @param [in] mcuType
 * @param [out] data
 * @return ErrorCode
 */
int // error code
MdrUartLoaderClient::getBootROM(Mcu::Type mcuType, QByteArray &data)
{
	if(mcuType==Mcu::Type::UndefinedMcu) return InvalidParametr;
	
	const uint32_t size = Mcu::mem[mcuType].romSize;
	const uint32_t addr = Mcu::mem[mcuType].romAddr;
	
	return read(addr, size, data);
}

/**
 * @brief MdrUartLoaderClient::getFlash
 * @param [in] mcuType
 * @param [out] data
 * @return ErrorCode
 */
int // error code
MdrUartLoaderClient::getFlash(Mcu::Type mcuType, QByteArray &data)
{
	if(mcuType==Mcu::Type::UndefinedMcu) return InvalidParametr;

	const uint32_t size = Mcu::mem[mcuType].flashSize;
	const uint32_t addr = Mcu::mem[mcuType].flashAddr;

	switch (mcuType) {
		case Mcu::Type::MDR1901VC1:
		case Mcu::Type::MDR1986BE9X: {
			return read(addr, size, data);
		} //break;
		case Mcu::Type::MDR1986BE1:
		case Mcu::Type::MDR1986BE3: {
			getVE1VE3flash(data);
		} break;
		case Mcu::Type::MDR1986BE4: {
			qDebug("Reading flash MDR1986BE4 not released yet.");
		} break;
		default:
			break;
	}
	return UnsupportedMcuType;
}


enum VE1 {
	RSTCLK_BASE     = 0x40020000u,
	RSTCLK_PERCLOCK = RSTCLK_BASE + 0x1C,
	
	PERCLOCK_EEPROM = 1u<<3,
	
	EEPROM_CNTRL_BASE = 0x40018000u,
	EEPROM_CMD = EEPROM_CNTRL_BASE + 0x0,
	EEPROM_ADR = EEPROM_CNTRL_BASE + 0x4,
	EEPROM_DI  = EEPROM_CNTRL_BASE + 0x8,
	EEPROM_DO  = EEPROM_CNTRL_BASE + 0xC,
	EEPROM_KEY = EEPROM_CNTRL_BASE + 0x10,
	
	EEPROM_KEY_VALUE = 0x8AAA5551u,
};

/**
 * @brief MdrUartLoaderClient::getVE1VE3flash
 * @param [out] data
 * @return ErrorCode
 */
int // error code
MdrUartLoaderClient::getVE1VE3flash(QByteArray &data) // Это лишнее.
// Загрузить стардартный загрузчик и читать, иного и не требовалось от этого проекта.
{
	int ec;
	uint32_t word;
	data.clear();
	
	qDebug("Only reading registers:");
	
	ec = readWordA(RSTCLK_PERCLOCK, word);
	qDebug("ec %d, RSTCLK_PERCLOCK = 0x%08X,", ec, word);
	qDebug("EEPROM cntrlr clocing is %s.", (word & PERCLOCK_EEPROM) ? "enabled":"disabled");
		
	ec = readWordA(EEPROM_CMD, word);
	qDebug("ec %d, EEPROM_CMD = 0x%08X,", ec, word);
	ec = readWordA(EEPROM_ADR, word);
	qDebug("ec %d, EEPROM_ADR = 0x%08X,", ec, word);
	ec = readWordA(EEPROM_DI, word);
	qDebug("ec %d, EEPROM_DI = 0x%08X,", ec, word);
	ec = readWordA(EEPROM_DO, word);
	qDebug("ec %d, EEPROM_DO = 0x%08X,", ec, word);
	ec = readWordA(EEPROM_KEY, word);
	qDebug("ec %d, EEPROM_KEY = 0x%08X,", ec, word);
	
	//data.resize(Mcu::mem[Mcu::Type::MDR1986BE1].flashSize);
	//data.fill('\xff', Mcu::mem[Mcu::Type::MDR1986BE1].flashSize);
	return ec;
}

/**
 * @brief MdrUartLoaderClient::getArmCpuid
 * @param [out] cpuid
 * @return ErrorCode
 */

int // error code
MdrUartLoaderClient::getArmCpuid(ARM::CPUID_t &cpuid)
{
	QByteArray cpuid_ba;
	int ec = read(ARM::CPUID_ADDR, 4, cpuid_ba);
	if(ec != NoError) {
		qDebug("getCPUID: Can't read, error code %d.", ec);
		return ec;
	}
	
	memcpy(static_cast<void*>(&cpuid), cpuid_ba.constData(), 4);
	//qDebug()<<"cpuid"<<cpuid_ba.toHex();
	//ARM::CPUID_t *cpuid = (ARM::CPUID_t *)&id;
	qDebug("CPUID: imp 0x%2X '%s', Arch 0x%X '%s', P/N 0x%3X '%s' r%dp%d",
	       cpuid.implementer, ARM::cpuidImplementerDecode(cpuid),
	       cpuid.arch,        (cpuid.arch == 0xC)?"ARMv6-M":(cpuid.arch == 0xF)?"EXT":"???",
	       cpuid.partNo,      ARM::cpuidPartNoDecode(cpuid),
	       cpuid.variant, cpuid.revision);
	return NoError;
}

/**
 * @brief MdrUartLoaderClient::getMdrMcuTypeAndRev
 * @param [out] mcuType Тип микроконтроллера фирмы Миландр.
 * @param [out] rev Ревизия МК.
 * @return ErrorCode
 */

int // error code
MdrUartLoaderClient::getMdrMcuTypeAndRev(Mcu::Type &mcuType, int &rev)
{
	mcuType = Mcu::Type::UndefinedMcu;
	rev = -1;

	ARM::CPUID_t cpuid;
	int ec = getArmCpuid(cpuid);
	if(ec != NoError) return ec;

	if(cpuid.implementer != ARM::CoreImplementer::ARM_Limited) return UnsupportedMcuType;

	QByteArray rom;
	uint32_t addr, id;

	switch (cpuid.partNo) {
		case ARM::CorePartNo::CM0: {
			mcuType = Mcu::Type::MDR1986BE4;
			rev = -1; // TODO
		} break;
		//case ARM::CorePartNo::CM0P: {} break;
		case ARM::CorePartNo::CM1: {
			// на ядре CM1 работаем пока только с ВЕ1 и ВЕ3, с одинакового размера BootROM.
			addr = 0xfff;
			ec = read(addr, 1, rom);
			if(ec != NoError) return ec;
			rev = static_cast<unsigned char>(rom.at(0)); // версия в кодировке байта в BootROM
			qDebug("CM1: rev byte 0x%X at addr 0x%X", rev, addr);
			if( (rev & 0xF) == 0xD) {
				//! https://forum.milandr.ru/viewtopic.php?p=7068#p7068
				mcuType = Mcu::Type::MDR1986BE1;
				switch (rev) {
					//case 0x00: rev = 1; break;
					case 0x8D: rev = 2; break;
					case 0x4D: rev = 3; break;
					case 0xCD: rev = 4; break;
					case 0x2D: rev = 5; break;
					case 0xAD: rev = 6; break;
					default: {
						rev = -2;
						ec = UndefinedMcuRevision;
					}
				}
			}
			else if( (rev & 0xF) == 0xA) {
				//! https://forum.milandr.ru/viewtopic.php?p=26672#p26672
				mcuType = Mcu::Type::MDR1986BE3;
				switch (rev) {
					//case 0x00: rev = 1; break;
					case 0x8A: rev = 2; break;
					case 0x4A: rev = 3; break;
					default: {
						rev = -2;
						ec = UndefinedMcuRevision;
					}
				}
			} else {
				qWarning("Unknown MCU type or VE1/VE3 revision 1.");
				mcuType = Mcu::Type::UndefinedMcu;
				if(rev == 0x0) {
					rev = 1;
					ec = UndefinedCM1;
				} else {
					rev = -1;
					ec = UnsupportedMcuType;
				}
			}
		} break; // ARM::CorePartNo::CM1
		case ARM::CorePartNo::CM3: {
			addr = 0x3FC;
			ec = read(addr, 4, rom);
			if(ec != NoError) return ec;
			memcpy(&id, rom.constData(), 4);
			qDebug("CM3: rev word 0x%08X at addr 0x%X", id, addr);
			/*
			 * https://forum.milandr.ru/viewtopic.php?p=27731#p27731
			 *
			 * 1901вц1: первая ревизия по адресу 0x000003FC загрузочного ПЗУ
			 * возвращает значение 0x02C00FDF. При этом
			 * вторая ревизия вернёт значение 0x82C00FDF, а актуальная
			 * третья значение 0x42C00FDF - то есть работа происходит
			 * аналогичным образом с 1Т/3Т.
			 *
			 * 1986ве9х первая ревизия 1986ВЕ9х по адресу 0x000003FC
			 * загрузочного ПЗУ вернет значение 0x03400FDF,
			 * когда как все следующие ревизии значение 0x83400FDF.
			 * То есть различить ревизии, начиная со второй, при таком подходе
			 * для 1986ВЕ9х не представляется возможным, необходимо руководствоваться
			 * маркировкой микроконтроллера на крышке корпуса и сопоставлять значения
			 * с приведенной таблицей из файла errata всех выпускаемых ревизий.*/
			if(      id == 0x02C00FDFu ) {
				mcuType = Mcu::Type::MDR1901VC1;
				rev = 1;
			}
			else if( id == 0x82C00FDFu ) {
				mcuType = Mcu::Type::MDR1901VC1;
				rev = 2;
			}
			else if( id == 0x42C00FDFu ) {
				mcuType = Mcu::Type::MDR1901VC1;
				rev = 3;
			}
			else if( id == 0x03400FDFu ) {
				mcuType = Mcu::Type::MDR1986BE9X;
				rev = 1;
			}
			else if( id == 0x83400FDFu ) {
				mcuType = Mcu::Type::MDR1986BE9X;
				rev = 2;
				qDebug("1986BE9x second or early revision.");
			}
			else {
				qWarning("Unknown MCU CM3 type or VC1/VE9x revision.");
				mcuType = Mcu::Type::UndefinedMcu;
				rev = -1;
				ec = UndefinedCM3;
			}
		} break; // ARM::CorePartNo::CM3
		//case ARM::CorePartNo::CM4: {} break;
		default: {
			mcuType = Mcu::Type::UndefinedMcu;
			rev = -1;
			return UnsupportedMcuType;
		}
	}
	return ec;
}


QByteArray // ответ от МК
MdrUartLoaderClient::portWR(const QByteArray &send,
                            int expectedResponseBytesCount,
                            int timeout)
{
	QElapsedTimer timer;
	QByteArray ba;
	if( !port->isOpen()) return ba;
	port->readAll(); // cp210x on Debian/Linux давал лишний байт 0xFF.
	port->write(send);
	port->waitForBytesWritten(timeout);
	timer.start();
	do {
		port->waitForReadyRead(timeout/2);
		ba.append(port->read(expectedResponseBytesCount));
		// WTF? QTimer::isActive() is not work!
		// See timerisactive.pro Qt project.
		// http://www.prog.org.ru/topic_31496_0.html
	} while( ba.size() < expectedResponseBytesCount && timer.elapsed() < timeout);
	return ba;
}

int // error code
MdrUartLoaderClient::send(uint32_t addr, const QByteArray &data)
{
	if( !port->isOpen()) return PortIsNotOpen;
	
	qDebug("send: addr 0x%X, size %d.", addr, data.size());
	if(addr==FORBIDDEN_PARAMETER_VALUE) return InvalidParametr;

	ByteArray req(1, char(CMD_LOAD));
	req.append(addr);
	req.append(static_cast<uint32_t>(data.size()));
	//qDebug()<<"send: raw header: size"<<req.size()<<req.toHex();
	
	QByteArray ans = portWR(req.q(), 1, 100);
	if(!ans.contains(char(CMD_LOAD))) {
		return ProtocolError;
	}
	if(ans.length()>1) {
		if(ans.at(0) == ERR_FLAG) {
			switch(ans.at(1)) {
			case ERR_CHN: return LoaderHwError;
			case ERR_CMD: return LoaderUnknownCmd;
			default: return LoaderUnknownErr;
			}
		} else {
			return ProtocolError;
		}
	}
	
	qint64 to_ms = data.size() + 1;
	to_ms =(to_ms *  1100 * 10 )/ port->baudRate() + 300; // [мс]
	//   кол.байт милли+рез бит   скорость[бит/с]   запас
	
	ans = portWR(data, 1, int(to_ms));
	if(!ans.contains(char(REPLY_OK))) {
		return ProtocolError;
	}
	if(ans.length()>1) {
		if(ans.at(0) == ERR_FLAG) {
			switch(ans.at(1)) {
			case ERR_CHN: return LoaderHwError;
			case ERR_CMD: return LoaderUnknownCmd;
			default: return LoaderUnknownErr;
			}
		} else {
			return ProtocolError;
		}
	}
	return NoError;
}

int MdrUartLoaderClient::writeAndVerify(uint32_t addr, const QByteArray& data)
{
	int ec = ping();
	if(ec != NoError) return ec;
	
	ec = send(addr, data);
	if(ec != NoError) return ec;
	
	QByteArray check;
	ec = read(addr, uint32_t(data.size()), check);
	if(ec != NoError) return ec;
	
	if(check != data) return DataTransferError;
	return NoError;
}

int MdrUartLoaderClient::run(uint32_t addr)
{
	if( !port->isOpen()) return PortIsNotOpen;
	
	qDebug("run: addr 0x%X.", addr);
	if(addr==FORBIDDEN_PARAMETER_VALUE) return InvalidParametr;

	ByteArray req(1, char(CMD_RUN));
	req.append(addr);
	//qDebug()<<"run: raw req"<<req.size()<<req.toHex();
	
	QByteArray ans = portWR(req.q(), 100, 200);
	qDebug("run: answer after RUN command '%s'", ans.constData());
	if(!ans.contains(char(CMD_RUN))) {
		return ProtocolError;
	}
	if(ans.length()>1) {
		if(ans.at(0) == ERR_FLAG) {
			switch(ans.at(1)) {
			case ERR_CHN: return LoaderHwError;
			case ERR_CMD: return LoaderUnknownCmd;
			default: return LoaderUnknownErr;
			}
		} else {
			return ProtocolError;
		}
	}
	return NoError;
}

#define U QString::fromUtf8

const QString MdrUartLoaderClient::ecToString(int e)
{
	const enum ErrorCode ec = static_cast<enum ErrorCode >(e);
	switch (ec) {
		case NoError: return U("Не проблема.");
		case CanNotOpenPort: return U("Невозможно открыть коммуникационный порт: ");//+ portErrorString()
		case PortIsNotOpen: return U("Порт не открыт.");
		case McuDoesNotAnswer: return U("Микроконтроллер не отвечает.");
		case LoaderHwError: return U("ROM-загрузчик: аппаратная ошибка.");
		case LoaderUnknownCmd: return U("ROM-загрузчик: неизвестная команда.");
		case LoaderUnknownErr: return U("ROM-загрузчик: неизвестная ошибка.");
		case InvalidParametr: return U("Недопустимый пареметр команды.");
		case DataTransferError: return U("Ошибка обмена данными.");
		case ProtocolError: return U("Ошибка в протоколе обмена данными.");
		case UnsupportedMcuType: return U("Неподдерживаемый тип микроконтроллера.");
		case UndefinedMcuRevision: return U("Невозможно определить ревизию МК.");
		case UndefinedCM1: return U("Неизвестный МК с ядром Cortex-M1.");
		case UndefinedCM3: return U("Неизвестный МК с ядром Cortex-M3.");
		default: return U("Неизвестный код ошибки!"); // NOTE тут и _должно_ быть предупреждение о том,
			// что switch покрывает все перечисления и ветвь default избыточна.
	}
}


