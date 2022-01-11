#include "vasililoader.h"
#include "bytearray.h"

#include <QDebug>
//#include <QThread>
#include <QElapsedTimer>
#include <QFile>
//#include <QMap>

#define MAX_CONNECTION_TRY (50)
#define SYNC_FRAME_LENGTH  (20)

#define CMD_CR             (0x0D)
#define CMD_SYNC           (0x00)

#define UART_LOADER_SPEED_DEFAULT (9600)

#define U QString::fromUtf8

// Exit If Error
/*
#define EIE(x) do { \
	if((x) != MdrUartLoaderClient::ErrorCode::NoError) { \
		qCritical("doWorkNew: At file %s, line %d: mucl ec %d - %s",\
			__FILE__, __LINE__, (x),mulc->ecToString(x).toLocal8Bit().constData()); \
		return false; \
	} \
} while (0);
*/

//static QMap<int,const char *> idStr;

const char * idStr(Mcu::Type m)
{
	switch (m) {
		default:
		case Mcu::Type::UndefinedMcu:return "Undefined";
		//case Mcu::Type::MDR1986BE1:  return "1986BOOTUART"; // FIXME 
		case Mcu::Type::MDR1986BE1:  return "MDR1986VE1";
		case Mcu::Type::MDR1986BE9X: return "1986BOOTUART";
		case Mcu::Type::MDR1986BE3:  return "1986BOOTUART";
		case Mcu::Type::MDR1986BE4:  return "BOOTUARTVE4";
		case Mcu::Type::MDR1901VC1:  return "1901VC1BOOTUART";
	}
}

Mcu::Type findMcuByIdStr(QByteArray id)
{
	Mcu::Type dev = Mcu::Type::UndefinedMcu;
	for (int i = 0; i < Mcu::Type::TotalMcuCount; i++)
	{
		dev = static_cast<Mcu::Type>(i);
		if(id.contains(idStr(dev))) {
			return dev;
		}
	}
	return Mcu::Type::UndefinedMcu;
}

VasiliLoaderClient::VasiliLoaderClient(QObject *parent) : QObject(parent),
	flasher(new Hex), firmware(new Hex),
	port(new QSerialPort), mulc(new MdrUartLoaderClient),
  work(NoWork), stop(false), mcuType(Mcu::UndefinedMcu),
  m_baseAddress(Hex::FORBIDDEN_ADDRESS)

{
	Q_INIT_RESOURCE(mdrloader);
}

VasiliLoaderClient::~VasiliLoaderClient()
{
	delete flasher;
	delete firmware;
	delete mulc;
	delete port;
}

bool VasiliLoaderClient::loadFirmware()
{
  int errCode;
  if( !QFile::exists(fwfn) ) {
      emit textChanged(U("Файл прошивки не найден."));
      return false;
  }
  errCode = firmware->loadFromFile(fwfn);
  if(Hex::EOF_DETECTED != errCode) {
      emit textChanged(U("Невозможно открыть файл прошивки, код ошибки %1, строка %2.")
                       .arg(errCode)
                       .arg(firmware->stringNumber()));
      return false;
  }
  // TODO: тестировать запись бинарного файла в МК.
  if( firmware->baseAdr() == Hex::FORBIDDEN_ADDRESS ) {
    if(m_baseAddress == Hex::FORBIDDEN_ADDRESS) {
      m_baseAddress = Mcu::mem[mcuType].flashAddr;
      qDebug("loadFirmware: set base address to begin FLASH memory.");
    } else {
      qDebug("loadFirmware: set base address externally 0x%X.", m_baseAddress);
    }
    firmware->setData(firmware->data(), m_baseAddress, Hex::FORBIDDEN_ADDRESS);
  }
  else {
    if( (firmware->baseAdr() < Mcu::mem[mcuType].flashAddr) ||
        (firmware->baseAdr() + firmware->sizeData() >
         Mcu::mem[mcuType].flashAddr + Mcu::mem[mcuType].flashSize) ) {
        emit textChanged(U("Загружаемая программа выходит за пределы диапазона flash-памяти! "
                            "Начинается 0x%1, заканчивается 0x%2")
                         .arg(firmware->baseAdr(), 8, 16, QChar('0'))
                         .arg(firmware->baseAdr()+firmware->sizeData(), 8, 16, QChar('0')));
        return false;
    }
    if( (firmware->baseAdr() & 0xFFU) != 0 ) {
        emit textChanged(U("Адрес начала записываемого кода должен быть кратен 256 байт! 0x%1")
                         .arg(firmware->baseAdr(), 8, 16, QChar('0')));
        return false;
    }
  }
  //qInfo - This function was introduced in Qt 5.5.
  qDebug("loadFirmware: base 0x%08x, start 0x%08x, image size %d byte(s).",
        firmware->baseAdr(), firmware->startAdr(), firmware->sizeData());
  return true;
}

/* В миландровском загрузчике (как и во флешере vasili)
   принято передавать как стартовый адрес
   - для 1986ВЕ9х: адрес таблицы векторов прерывания;
   - для ВЕ1, ВЕ3: адрес первой исполняемой инструкции.
*/

uint32_t VasiliLoaderClient::getFlasherStartAddress()
{
	uint32_t startAddress;
	switch (mcuType) {
		case Mcu::MDR1986BE9X:
		case Mcu::MDR1901VC1:
			startAddress = flasher->baseAdr();
			break;
		case Mcu::MDR1986BE1:
		case Mcu::MDR1986BE3:
		case Mcu::MDR1986BE4:
			memcpy(static_cast<void*>(&startAddress),
						 &(flasher->data().constData()[4]),  4);
			startAddress &= ~1U;
			break;
		default:
			startAddress = MdrUartLoaderClient::FORBIDDEN_PARAMETER_VALUE;
	}
	return startAddress;
}

bool VasiliLoaderClient::loadFlasher()
{
  int errCode;
  bool internalFlasher = false;
  if(flasherfn.isEmpty()) {
      switch (mcuType) {
      case Mcu::MDR1986BE9X:
          flasherfn = ":/flashers/vasili/1986_BOOT_UART.hex";
          break;
      case Mcu::MDR1986BE1:
      case Mcu::MDR1986BE3:
//        flasherfn = ":/flashers/vasili/1986ve1bootuarte.hex"; // FIXME
          flasherfn = ":/flashers/legacy/1986ve1bootuarte.elf";
          break;
      case Mcu::MDR1986BE4:
          flasherfn = ":/flashers/vasili/bootuartve4.hex";
          break;
      case Mcu::MDR1901VC1:
          flasherfn = ":/flashers/vasili/1901VC1_BOOT_UART.hex";
          break;
      default:
          return false;
      }
      internalFlasher = true;
  }
  if( !QFile::exists(flasherfn) ) {
      emit textChanged(U("Файл флешера не найден."));
      return false;
  }
  errCode = flasher->loadFromFile(flasherfn);
  qDebug("loadFlasher: Hex::loadFromFile: errCode %d, flasher filneme '%s'.",
         errCode, flasherfn.toLocal8Bit().constData());
  if(Hex::EOF_DETECTED != errCode) {
      emit textChanged(U("Ошибка при открытии *.hex файла флешера, "
                          "код ошибки %1, строка %2.")
                       .arg(errCode)
                       .arg(flasher->stringNumber()));
      return false;
  }
	if(flasher->baseAdr() == Hex::FORBIDDEN_ADDRESS) {
		if(internalFlasher) {
			switch (mcuType) {
//			case Mcu::MDR1986BE9X:
//			case Mcu::MDR1901VC1:
//					break;
			case Mcu::MDR1986BE1:
			case Mcu::MDR1986BE3:
					flasher->setData(flasher->data(), Mcu::mem[mcuType].xramAddr, Hex::FORBIDDEN_ADDRESS);
					break;
//			case Mcu::MDR1986BE4:
//					break;
			default:
				qDebug("loadFlasher: TODO: Add internal image flasher .bin format support for this MCU type.");
				return false;
			}
		}
		else {
			qDebug("loadFlasher: TODO: Add binary flasher format support (setting base address).");
			return false;
		}
		qDebug("loadFlasher: set internal flasher base address manually, 0x%X", flasher->baseAdr());
	}
  if( (flasher->baseAdr() < Mcu::mem[mcuType].xramAddr) || 
      (flasher->baseAdr() + quint32(flasher->sizeData()) >
       Mcu::mem[mcuType].xramAddr + Mcu::mem[mcuType].xramSize) ) {
      emit textChanged(U("Загружаемая программа выходит за пределы диапазона XRAM-памяти! "
                          "Начинается 0x%1, заканчивается 0x%2")
                       .arg(flasher->baseAdr(), 8, 16, QChar('0'))
                       .arg(flasher->baseAdr()+flasher->sizeData(), 8, 16, QChar('0')));
      return false;
  }
  qDebug("loadFlasher: base = 0x%X start = 0x%X size = %d",
         flasher->baseAdr(), flasher->startAdr(), flasher->sizeData() );
  return true;
}

QByteArray VasiliLoaderClient::portWR(const QByteArray &send, int bytesCount, int timeout)
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
        ba.append(port->read(bytesCount));
        // WTF? QTimer::isActive() is not work!
        // See timerisactive.pro Qt project.
        // http://www.prog.org.ru/topic_31496_0.html
    } while( (ba.size() < bytesCount) && (timer.elapsed() < timeout) );
    return ba;
}

bool VasiliLoaderClient::checkAndLoadFiles()
{
    // =================== Загружаем файл прошивки
    if(work&Program || work&Verify ) {
      if(loadFirmware() == false) {
        return false;
      }
    }
    if(work&Dump) {
      // nothing to do?
    }
    // =================== Загружаем файл флешера
    return loadFlasher();
}


QByteArray VasiliLoaderClient::getIdStr()
{
	ByteArray request(1, 'I');
	return portWR(request.q(), 100, 100);;
}

bool VasiliLoaderClient::checkIdStr()
{
	QByteArray respond = getIdStr();
	if(!respond.contains(idStr(mcuType))) {
		qDebug("checkIdStr: expected id string '%s'.", idStr(mcuType));
		return false;
	}
	return true;
}


int // MdrUartLoaderClient::ErrorCode
VasiliLoaderClient::tryConnect(Mcu::Type& dev, int& rev)
{
	int ec = mulc->openPort(portname);
	if(ec != MdrUartLoaderClient::ErrorCode::NoError) {
		emit statusChanged(mulc->ecToString(ec));
		emit textChanged(U("Невозможно открыть порт: ")+mulc->ecToString(ec));
		return ec;
	}
	// =================== Проверка предыдущей синхронизации
	ec = mulc->ping();
	if(ec == MdrUartLoaderClient::ErrorCode::McuDoesNotAnswer) {
		// =================== Синхронизация
		ec = mulc->sync();
		if(ec != MdrUartLoaderClient::ErrorCode::NoError) {
			emit stopButtonSetEnable(true);
			emit statusChanged(U("Нет ответа от МК."));
			emit textChanged(U("Нет ответа от МК: ") + mulc->ecToString(ec));
			return ec;
		}
	}
	emit stopButtonSetEnable(true);
	// =================== Проверка связи
	ec = mulc->ping();
	if(ec == MdrUartLoaderClient::ErrorCode::NoError) {
		emit statusChanged(U("МК ответил."));
		emit textChanged(U("Есть синхронизация."));
	} else {
		emit statusChanged(U("МК НЕ ответил."));
		emit textChanged(U("Проверка связи на стандартной скорости не прошла: ")
		                 + mulc->ecToString(ec));
		return ec;
	}
	// =================== Получение типа и ревизии МК
	ec = mulc->getMdrMcuTypeAndRev(dev, rev);
	if(ec != MdrUartLoaderClient::ErrorCode::NoError &&
	   ec != MdrUartLoaderClient::ErrorCode::UndefinedMcuRevision) {
		emit statusChanged(U("МК не определён."));
		emit textChanged(U("Тип и/или ревизия МК не определены: ")
		                 + mulc->ecToString(ec)
		                 + U("Тип '%1', ревизия '%2'.").arg(Mcu::name[dev]).arg(rev));
		return ec;
	}
	else {
		emit statusChanged(U("МК определён."));
		emit textChanged(U("Определены тип и ревизия МК: '%1', ревизия '%2'.")
		                 .arg(Mcu::name[dev]).arg(rev));
	}
	return ec;
}


bool VasiliLoaderClient::dump()
{
    int chankCount;
    unsigned char ks;
    ByteArray d;
    QByteArray respond, dump;
    
    if(dumpfn.isEmpty()) {
        emit textChanged(U("Задайте имя файла для сохранения дампа прошивки "
                            "или снимите флажок 'Считать'."));
        return false;
    }
    
    if( !(dumpfn.endsWith(".bin", Qt::CaseInsensitive)||
          dumpfn.endsWith(".hex", Qt::CaseInsensitive)) ) dumpfn.append((".hex"));
    emit dumpFilenameChanged(dumpfn);
    /** Изначально рассчёт был проверять на возможность программы открыть файл на запись
     * ПЕРЕД скачиванием прошивки,
     * но поскольку невозможность открыть файл - это ошибка (т.е. не штатная ситуация),
     * то не грех разок считать прошику в холостую...
    dumpFile = new QFile;
    dumpFile->setFileName(dumpfn);
    if( !dumpFile->open(QIODevice::WriteOnly) ) {
        emit textChanged(U("Не возможно открыть Файл для сохранения дампа прошивки: ")
                         .append(dumpFile->errorString()));
        delete dumpFile;
        dumpFile = 0;
        return false;
    } */
    emit statusChanged(U("Считывание прошивки..."));
    qDebug("dump...");
    quint32 fAdr = Mcu::mem[mcuType].flashAddr;
    quint32 fSz  = Mcu::mem[mcuType].flashSize;
    d = "A";
    d.append(fAdr);
    respond = portWR(d.q(), 1, 100);
    ks  = (unsigned char)d.at(1);
    ks += (unsigned char)d.at(2);
    ks += (unsigned char)d.at(3);
    ks += (unsigned char)d.at(4);
    if( respond.isEmpty() || (respond.at(0) != ks) ) {
        emit textChanged(U("Не удалось задать адрес для считывания прошивки."));
        return false;
    }
    dump.clear();
    dump.reserve(fSz);
    chankCount = fSz / 8;
    d = "V";
    for(int chank = 0; chank < chankCount; chank++ ) {
        respond = portWR(d.q(), 8, 100);
        if(respond.size() == 8) {
            dump.append(respond);
        } else {
            emit textChanged(U("Проблема при считывания прошивки на %1%.")
                             .arg((double)chank/chankCount*100.0));
            return false;
        }
        if( (chank & 0x1F) == 0) {
            emit statusChanged(U("считываниe прошивки на %1%...")
                               .arg((double)chank/chankCount*100.0, 2, 'f', 1));
            if(stop) {
                emit textChanged(U("Операция отменена пользователем."));
                return false;
            }
        }
    }
    emit statusChanged(U("Запись дампа в файл..."));
/*    if(dumpFile) {
        dumpFile->write(dump);
        dumpFile->waitForBytesWritten(5000);
        dumpFile->close();
        //qDebug() << "Hex::saveToFile =" <<
        Hex::saveToFile(dumpFile->fileName().append(".hex"), dump, 
                        Mcu::mem[mcuType].flashAdr, Mcu::mem[mcuType].flashAdr);
        dumpFile->close();
        delete dumpFile;
        dumpFile = 0;
    } else {
        emit textChanged(U("Не был создан Файл для сохранения дампа прошивки!"));
        return false;
    } */
    Hex::FileParseError err = Hex::saveToFile(dumpfn, dump, fAdr, fAdr);
    QString s;
    if(err != Hex::NO_ERROR) {
        s=U("Не удалось записать в файл, код ошибки %1.").arg((int)err);
        emit statusChanged(s);
        emit textChanged(s);
        return false;
    }
    s=U("Считывание %1 КБ прошивки прошло успешно.").arg(fSz>>10);
    emit statusChanged(s);
    emit textChanged(s);
    return true;
}

bool VasiliLoaderClient::erase()
{
    ByteArray d;
    QByteArray respond;
    unsigned char ks;
    qDebug("ERASE...");
    // TODO: реализовать не полное стирание флешки (в прошивке ВЕ9х тоже
    if(stop) {
        emit textChanged(U("Операция отменена пользователем."));
        return false;
    }
    emit statusChanged(U("Стирание ВСЕЙ flash-памяти..."));
    d = "E";
    switch (mcuType) {
    case Mcu::MDR1986BE9X:
    case Mcu::MDR1901VC1:
        break;
    case Mcu::MDR1986BE1:
    case Mcu::MDR1986BE3:
        d.append(static_cast<unsigned int>(0x0));
        ks = 0;
        break;
    case Mcu::MDR1986BE4:
        //
        break;
    default:
        break;
    }
    respond = portWR(d.q(), 1, 100);
    if(mcuType == Mcu::MDR1986BE1 || mcuType == Mcu::MDR1986BE3) {
        qDebug("erase BE1 param respond[%d] '%s'.",
               respond.size(), respond.toHex().constData());
        if( (respond.size()<1) || (respond.at(0) != ks) ) {
            emit textChanged(U("Параметры команды очистки не прошли."));
        }
    }
    respond.clear();
    respond = portWR(respond, 9, 2000);
    if( (respond.size()<9) || (respond.at(0) != 'E') ) {
        emit textChanged(U("Команда очистки не прошла."));
        qDebug("erase: fail with req %s resp[%d] 0x'%s'",
               d.constData(), respond.size(), respond.toHex().constData());
        return false;
    }
    quint32 addr, data;
    addr =            (unsigned char)respond.at(4);
    addr = addr*256 + (unsigned char)respond.at(3);
    addr = addr*256 + (unsigned char)respond.at(2);
    addr = addr*256 + (unsigned char)respond.at(1);
    
    data =            (unsigned char)respond.at(8);
    data = data*256 + (unsigned char)respond.at(7);
    data = data*256 + (unsigned char)respond.at(6);
    data = data*256 + (unsigned char)respond.at(5);
    
    if( (addr != Mcu::mem[mcuType].flashAddr + Mcu::mem[mcuType].flashSize)
     || (data != Hex::FORBIDDEN_ADDRESS)) {
        emit textChanged(U("При стирании flash-памяти произошла ошибка"
                            " по адресу 0x%1 остались данные 0x%2.")
                         .arg(addr, 8, 16, QChar('0'))
                         .arg(data, 8, 16, QChar('0')));
        return false;
    }
    emit statusChanged(U("Стирание ВСЕЙ flash-памяти прошло успешно."));
    emit textChanged(U("Стирание ВСЕЙ flash-памяти прошло успешно."));
    return true;
}

bool VasiliLoaderClient::program()
{
    int chankCount;
    unsigned char ks;
    ByteArray d;
    QByteArray ba, respond;

    emit statusChanged(U("Программирование %1 байт flash-памяти...")
                       .arg(firmware->sizeData()));
    emit textChanged(U("Программирование %1 байт flash-памяти "
                        "с адреса 0x%2...")
                     .arg(firmware->sizeData())
                     .arg(firmware->baseAdr(), 8, 16, QChar('0')));
    qDebug("programm...");
    // =================== Задание адреса для программирования flash
    d = "A";
    d.append(firmware->baseAdr());
    respond = portWR(d.q(), 1, 100);
    ks  = (unsigned char)d.at(1);
    ks += (unsigned char)d.at(2);
    ks += (unsigned char)d.at(3);
    ks += (unsigned char)d.at(4);
    if( respond.isEmpty() || (respond.at(0) != ks) ) {
        emit textChanged(U("Не удалось задать адрес для заиписи прошивки."));
        return false;
    }
    // =================== Загрузка данных частями
    chankCount = (firmware->sizeData()+255) /256;
    d = "P";
    for(int chank = 0; chank < chankCount; chank++ ) {
        if(stop) {
            emit textChanged(U("Операция отменена пользователем."));
            return false;
        }
        ba = firmware->data().mid(256*chank, 256);
        int size = ba.size();
        if(size < 256) ba.append(QByteArray(256-size, (char)0xFF));
        unsigned char ks = 0;
        for(int i=0; i<ba.size(); i++) ks+=(unsigned char)ba.at(i);
        respond = portWR(ba.prepend(d.q()), 1, 300);
        if( (respond.size() < 1 ) || ((unsigned char)respond.at(0) != ks) ) {
            emit textChanged(U("Проблема при передаче данных на %1%.")
                             .arg((100.0*chank)/chankCount));
            qDebug("program: respond size %d, data 0x%s, calculated ks = %d, request size %d.",
						       respond.size(), respond.toHex().constData(), ks, ba.size());
            return false;
        }
        emit statusChanged(U("программирование прошивки на %1%...")
                           .arg((100.0*chank)/chankCount, 2, 'f', 1));
    }
    emit statusChanged(U("Программирование прошивки прошло успешно."));
    emit textChanged(U("Программирование прошивки прошло успешно."));
    return true;
}

bool VasiliLoaderClient::verify()
{
    int chankCount;
    unsigned char ks;
    ByteArray d;
    QByteArray ba, respond;
    
    emit statusChanged(U("Верификация %1 байт flash-памяти...")
                       .arg(firmware->sizeData()));
    emit textChanged(U("Верификация %1 байт flash-памяти "
                        "с адреса 0x%2...")
                     .arg(firmware->sizeData())
                     .arg(firmware->baseAdr(), 8, 16, QChar('0')));
    qDebug("verifieng...");
    d = "A";
    d.append(firmware->baseAdr());
    respond = portWR(d.q(), 1, 100);
    ks  = (unsigned char)d.at(1);
    ks += (unsigned char)d.at(2);
    ks += (unsigned char)d.at(3);
    ks += (unsigned char)d.at(4);
    if( respond.isEmpty() || (respond.at(0) != ks) ) {
        emit textChanged(U("Не удалось задать адрес для заиписи прошивки."));
        return false;
    }
    chankCount = (firmware->sizeData()+7) / 8;
    d = "V";
    for(int chank = 0; chank < chankCount; chank++ ) {
        ba = firmware->data().mid(8*chank, 8);
        int size = ba.size();
        if(size < 8) ba.append(QByteArray(8-size, (char)0xFF));
        respond = portWR(d.q(), 8, 100);
        if(respond.size() < 8) {
            emit textChanged(U("Проблема при передаче данных на %1%.")
                             .arg((double)chank/chankCount*100.0));
            qDebug("verify: fail with respond [%d] '%s', at chank %d.",
                   respond.size(), respond.toHex().constData(), chank);
            return false;
        }
        if(respond != ba) {
            emit textChanged(U("Ошибка верификации данных на %1%.")
                             .arg((double)chank/chankCount*100.0));
            emit textChanged(U("По адресу 0x%1 считано %2, должно быть %3.")
                             .arg(firmware->baseAdr()+chank*8, 8, 16, QChar('0'))
                             .arg(QString(respond.toHex().toUpper()))
                             .arg(QString(ba.toHex().toUpper())) );
            return false;
        }
        if( (chank & 0x1F) == 0) {
            emit statusChanged(U("верификация прошивки на %1%...")
                               .arg((double)chank/chankCount*100.0, 2, 'f', 1));
            if(stop) {
                emit textChanged(U("Операция отменена пользователем."));
                return false;
            }
        }
    }
    emit statusChanged(U("Верификация прошивки прошла успешно."));
    emit textChanged(U("Верификация прошивки прошла успешно."));
    return true;
}

bool VasiliLoaderClient::runFW()
{
    ByteArray req;
    QByteArray respond;
    // TODO: тестировать на всех, кроме ВЕ9х!
    qDebug("RUN...");
    emit statusChanged(U("Передача управления прошивке МК..."));
    req = "R";
    //d.append(Mcu::mem[mcuType].flashAdr);
    respond = portWR(req.q(), 1, 100);
    qDebug("runFW: req[%d] '%s', rsep[%d] '%s'.",
           req.size(), req.toHex().constData(),
           respond.size(), respond.toHex().constData());
    if( respond.isEmpty() || (respond.at(0) != 'R') ) {
        emit textChanged(U("Ошибка обмена."));
        return false;
    }
    emit textChanged(U("Исполняется программа из flash памяти по адресу 0x%1.")
                     .arg(Mcu::mem[mcuType].flashAddr, 8, 16, QChar('0')));
    return true;
}

/* bool VasiliLoaderClient::doWork(VasiliLoaderClient::WorkType w)
{
    this->work = w;
    stop = false;
    
    if( (mcuType != Mcu::MDR1986BE9X)&&(mcuType != Mcu::MDR1901VC1)
      &&(mcuType != Mcu::MDR1986BE1 )&&(mcuType != Mcu::MDR1986BE3)
      &&(mcuType != Mcu::MDR1986BE4) ) {
        return false;
    }
    
    bool retval = checkAndLoadFiles();
    
    //port = new QSerialPort;
    
    if(retval) retval &= checkConnect();
    
    if( (work & Dump) && retval) {
        retval &= dump();
    }
    if( (work & Erase) && retval) {
        retval &= erase();
    }
    if( (work & Program) && retval) {
        retval &= program();
    }
    if( (work & Verify) && retval) {
        retval &= verify();
    }
    if( (work & Run) && retval) {
        retval &= runFW();
    }
//    if(dumpFile) {
//        dumpFile->close();
//        delete dumpFile;
//        dumpFile=0;
//    }
    //port->close();
    //delete port;
    if(retval) emit statusChanged(U("Всё прошло успешно!"));
    emit workFinished();
    return retval;
} */

bool VasiliLoaderClient::doWorkNew(VasiliLoaderClient::WorkType w)
{
	this->work = w;
	stop = false;
	int ec, rev = -100;
	Mcu::Type dev;
	bool retval = true;
	bool flasherAlreadyLoaded = false;
	
	if(mcuType == Mcu::Type::UndefinedMcu) {
		ec = tryConnect(dev, rev);
		if(ec != MdrUartLoaderClient::NoError) {
			QString s = MdrUartLoaderClient::ecToString(ec);
			emit statusChanged(s);
			if(ec == MdrUartLoaderClient::CanNotOpenPort) {
				s.append(mulc->portErrorString());
			}
			emit textChanged(s);
			goto endWork;
		}
		// mulc->closePort();
		if(dev == Mcu::Type::UndefinedMcu) {
			QString s = MdrUartLoaderClient::ecToString(
			              MdrUartLoaderClient::UnsupportedMcuType);
			emit statusChanged(s);
			emit textChanged(s);
			qCritical("doWorkNew: Mcu::Type::UndefinedMcu");
			goto  endWork;
		}
		mcuType = dev;
		
		retval = checkAndLoadFiles();
	}
	else {
		if( !checkAndLoadFiles()) {
			qCritical("doWorkNew: checkAndLoadFiles");
			goto endWork;
		}
		port->setPortName(portname);
		port->setBaudRate(baudrate);
		if( !port->open(QIODevice::ReadWrite) ) {
			emit statusChanged(port->errorString());
			emit textChanged(U("Невозможно открыть порт: ")+port->errorString());
			goto endWork; //return false;
		}
		// =================== Привентивное чтение id-строки.
		// Это возможно в случае уже загруженного и исполняемого прошивальщика.
		QByteArray id = getIdStr();
		
		// ROM-загрузчик не знает команды 'I', отвечает 'E'.
		if(id == QByteArray(1, char(MdrUartLoaderClient::ERR_FLAG))) id.clear();
		
		if(id.isEmpty()) {
			port->close();
			ec = tryConnect(dev, rev);
			if(ec != MdrUartLoaderClient::NoError) {
				QString s = MdrUartLoaderClient::ecToString(ec);
				emit statusChanged(s);
				if(ec == MdrUartLoaderClient::CanNotOpenPort) {
					s.append(mulc->portErrorString());
				}
				emit textChanged(s);
				goto endWork;
			}
			if(dev != mcuType) {
				qCritical("doWorkNew: Unexpected MCU type: "
				          "founded '%s' (%d), rev %d; must be '%s'.",
				          Mcu::name[dev], dev, rev, Mcu::name[mcuType]);
				QString s = MdrUartLoaderClient::ecToString(
				              MdrUartLoaderClient::UnsupportedMcuType);
				emit statusChanged(s);
				emit textChanged(s);
				goto endWork;
			}
		}
		else {
			if(mcuType != findMcuByIdStr(id)) {
				qCritical("doWorkNew: Unexpected flasher(MCU) type: "
				          "founded '%s' (%s, 0x%s)",
				          Mcu::name[findMcuByIdStr(id)],
				          id.constData(),  id.toHex().constData());
				emit statusChanged(U("Неподдерживаемый прошивальщик"));
				emit textChanged(U("Неподдерживаемый прошивальщик: "
				                   "ожидаемая строка '%1', получено '%2' (0x%3).")
				                 .arg(Mcu::name[findMcuByIdStr(id)])
				                 .arg(QString(id)).arg(QString(id.toHex())));
				goto endWork; //return false;
			}
			flasherAlreadyLoaded = true;
		}
	}
	
	if(work == NoWork) {
		mulc->closePort();
		printf("Found device %s, revision %d.\n", Mcu::name[mcuType], rev);
		QString s = U("Работа не задана, определён МК %1 ревизия %2.")
		            .arg(QString(Mcu::name[mcuType])).arg(rev);
		emit statusChanged(s);
		emit textChanged(s);
		emit workFinished();
		return true;
	}
	emit stopButtonSetEnable(true);

	if(false == flasherAlreadyLoaded) {
		//qDebug("----- doWorkNew: setSpeed, load and run flasher!");
		if(baudrate != UART_LOADER_SPEED_DEFAULT) {
			// =================== Установка скорости (по необходимости)
			ec = mulc->setSpeed(baudrate);
			if(ec != MdrUartLoaderClient::ErrorCode::NoError) {
				emit statusChanged(mulc->ecToString(ec));
				emit textChanged(mulc->ecToString(ec));
				return false;
			}
			// =================== Проверка связи на новой скорости
			ec = mulc->ping();
			if(ec == MdrUartLoaderClient::ErrorCode::NoError) {
				emit statusChanged(U("МК работает на новой скорости."));
				emit textChanged(U("Проверка связи на новой скорости прошла успешно."));
			} else {
				emit statusChanged(U("МК НЕ работает на новой скорости."));
				emit textChanged(U("Проверка связи на новой скорости НЕ прошла: ")
				                 + mulc->ecToString(ec));
				return false;
			}
		}
		// =================== Загрузка флешера в память
		ec = mulc->writeAndVerify(flasher->baseAdr(), flasher->data());
		if(ec != MdrUartLoaderClient::ErrorCode::NoError) {
			emit textChanged(U("Не прошла команда загрузки или верификации флешера: ")
			                 + mulc->ecToString(ec));
			// TODO emit status
			return false;
		}
		// =================== Запуск флешера
		ec = mulc->run(getFlasherStartAddress());
		if(ec != MdrUartLoaderClient::ErrorCode::NoError) {
			emit textChanged(U("Не прошла команда запуска флешера: ")
			                 + mulc->ecToString(ec));
			// TODO emit status
			qDebug("checkConnect: mulc->run error code %d", ec);
			return false;
		}
		mulc->closePort();
		port->setPortName(portname);
		port->setBaudRate(baudrate);
		if( !port->open(QIODevice::ReadWrite) ) {
			emit statusChanged(port->errorString());
			emit textChanged(port->errorString());
			return false;
		}
		// =================== Проверка запуска флешера чтением id-строки
		if(!checkIdStr()) {
			emit textChanged(U("ошибка идентификации загрузчика!"));
			// TODO emit status
			port->close();
			return false;
		}
	}
	emit textChanged(U("Флешер работает."));
	
	if( (work & Dump) && retval) {
		retval &= dump();
	}
	if( (work & Erase) && retval) {
		retval &= erase();
	}
	if( (work & Program) && retval) {
		retval &= program();
	}
	if( (work & Verify) && retval) {
		retval &= verify();
	}
	if( (work & Run) && retval) {
		retval &= runFW();
	}
	if(retval) emit statusChanged(U("Всё прошло успешно!"));
	port->close();
	emit workFinished();
	return retval;

endWork:
	mulc->closePort();
	port->close();
	emit stopButtonSetEnable(true);
	emit workFinished();
	return false;
}




