#include "hex.h"
#include "bytearray.h"
#include <QDebug>
#include <QFile>
#include <QBuffer>
#include "elf.h"

Hex::Hex() : start(0), base(0), stringNum(0), error(NOT_OPENED)
{    
}

Hex::FileParseError Hex::loadFromFile(QString fileName)
{
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly) ) {
        qDebug()<<file.fileName()<<file.errorString();
        error= CANNOT_OPEN_FILE;
        return error;
    }
    d.clear();
		if(fileName.endsWith(".elf", Qt::CaseInsensitive) ||
		   fileName.endsWith(".axf", Qt::CaseInsensitive) ) {
        QBuffer buf(&d);
        error = fromElf(&file, buf, &base, &start);
				stringNum = 0;
        return error;
    }
    if(fileName.endsWith(".hex", Qt::CaseInsensitive)) {
        d.reserve(file.size()/2);
        QBuffer buf(&d);
        error = fromHex(&file, &buf, &base, &start, &stringNum);
        return error;
    }
    if(fileName.endsWith(".bin", Qt::CaseInsensitive)) {
        d = file.readAll();
        file.close();
        base =  0xFFFFFFFF;
        start = 0xFFFFFFFF;
        stringNum = d.size();
        return error = EOF_DETECTED;
    }
    qDebug()<<"UNSUPPORTED_FILE_FORMAT"<<fileName;
    return UNSUPPORTED_FILE_FORMAT;
}

Hex::FileParseError Hex::fromElf(QIODevice *src, QBuffer &dest,
                                 quint32 *baseAdr, quint32 *startAdr)
{
	size_t elem; 
	Elf32_Ehdr hdr; // ELF file header.
	
	*baseAdr = 0; *startAdr = 0;
  if( !src->isOpen() || !(src->openMode() & QIODevice::ReadOnly) ) {
      src->close();
			if( !src->open(QIODevice::ReadOnly) ) {
          QFile *file(qobject_cast<QFile *>(src));
          if(file) qDebug()<<file->fileName()<<file->errorString();
          return NOT_OPENED_SOURCE_FILE;
      }
  }
  if( !dest.isOpen() || !(dest.openMode() & QIODevice::WriteOnly) ) {
      dest.close();
			if( !dest.open(QIODevice::WriteOnly) ) {
          src->close();
          QFile *file(qobject_cast<QFile *>(&dest));
          if(file) qDebug()<<file->fileName()<<file->errorString();
          return NOT_OPENED_DESTINATION_FILE;
      }
  }
	
	elem = src->read((char *)&hdr, sizeof(Elf32_Ehdr));
	if(elem != sizeof(Elf32_Ehdr)) {
		qDebug("Error: read elf header, error = %s.", src->errorString().toLocal8Bit().constData());
		return EMPTY_LINE_IS_READED;
	}
	
	if(hdr.e_ident[EI_MAG0] != ELFMAG0 ||
	   hdr.e_ident[EI_MAG1] != ELFMAG1 ||
	   hdr.e_ident[EI_MAG2] != ELFMAG2 ||
	   hdr.e_ident[EI_MAG3] != ELFMAG3 ) {
		qDebug("Error: magic number not found.");
		return UNSUPPORTED_FILE_FORMAT;
	}
	if(hdr.e_ident[EI_CLASS] != ELFCLASS32) {
		qDebug("Error: not supported class, %d.", hdr.e_ident[EI_CLASS]);
		return UNSUPPORTED_FILE_FORMAT;
	}
	if(hdr.e_ident[EI_DATA] != ELFDATA2LSB) {
		qDebug("Error: not supported dataorder, %d.", hdr.e_ident[EI_DATA]);
		return UNSUPPORTED_FILE_FORMAT;
	}
	if(hdr.e_ident[EI_VERSION] != EV_CURRENT) {
		qDebug("Error: not supported VERSION, %d.", hdr.e_ident[EI_VERSION]);
		return UNSUPPORTED_FILE_FORMAT;
	}
	
	
	if(hdr.e_phentsize != sizeof(Elf32_Phdr)) {
		qDebug("Error: program header size from elf header mismatch whith sizeof(elf32_phdr)");
		return UNSUPPORTED_FILE_FORMAT;
	}
	if(hdr.e_shentsize != sizeof(Elf32_Shdr)) {
		qDebug("Error: section header size from elf header mismatch whith sizeof(elf32_shdr)");
		return UNSUPPORTED_FILE_FORMAT;
	}
	
	if(hdr.e_phnum == 0) {
		qDebug("Error: program headers count is ZERO.");
		return UNSUPPORTED_FILE_FORMAT;
	}
	if(hdr.e_phnum > 1) {
		qDebug("Info: program headers count is %d. Using first omly!\n", hdr.e_phnum);
	}
	
	Elf32_Phdr phdr[hdr.e_phnum];
	if( !src->seek(hdr.e_phoff)) {
		qDebug("Error: seek fail, error = %s.", src->errorString().toLocal8Bit().constData());
		return EMPTY_LINE_IS_READED;
	}
	elem = src->read((char*)phdr, sizeof(Elf32_Phdr) * hdr.e_phnum);
	if(elem != sizeof(Elf32_Phdr) * hdr.e_phnum) {
		qDebug("Error: read program header(s), error = %s.", src->errorString().toLocal8Bit().constData());
		return EMPTY_LINE_IS_READED;
	}
	
	if(phdr[0].p_type != PT_LOAD) {
		qDebug("Error: first program headers is not PT_LOAD type, %d.", phdr[0].p_type);
		return UNSUPPORTED_FILE_FORMAT;
	}
	if( !src->seek(phdr[0].p_offset) ) {
		qDebug("Error: seek fail at read data, error = %s.", src->errorString().toLocal8Bit().constData());
		return EMPTY_LINE_IS_READED;
	}
	
	dest.buffer().resize(phdr[0].p_filesz);
	elem = src->read(dest.buffer().data(), phdr[0].p_filesz);
	if(elem != phdr[0].p_filesz) {
		qDebug("Error: read data to buffer, ferror = %s.", src->errorString().toLocal8Bit().constData());
		return EMPTY_LINE_IS_READED;
	}
	*startAdr = hdr.e_entry;
	*baseAdr = phdr[0].p_paddr;
	return EOF_DETECTED;
}

Hex::FileParseError Hex::fromHex(QIODevice *src, QIODevice *dest,
                                 quint32 *baseAdr, quint32 *startAdr, quint32 *stringNumber)
{
    quint32 dataType = 0, offset = 0, newBase, newStart, currOffset;
    quint32 adrPtr = 0, dataSize = 0;
    *baseAdr = 0; *startAdr = 0; *stringNumber = 0;
    if( !src->isOpen()) {
        if( !src->open(QIODevice::ReadOnly) ) {
            QFile *file(qobject_cast<QFile *>(src));
            if(file) qDebug()<<file->fileName()<<file->errorString();
            return NOT_OPENED_SOURCE_FILE;
        }
    }
    if( !dest->isOpen()) {
        if( !dest->open(QIODevice::WriteOnly) ) {
            src->close();
            QFile *file(qobject_cast<QFile *>(dest));
            if(file) qDebug()<<file->fileName()<<file->errorString();
            return NOT_OPENED_DESTINATION_FILE;
        }
    }
    FileParseError status = EOF_ISNT_DETECTED;
    while( !src->atEnd() && (status == EOF_ISNT_DETECTED) ) {
        /// Чтение и проверка строки
        QByteArray line = src->readLine();
        stringNumber++;
        if(line.isEmpty())     { status= EMPTY_LINE_IS_READED; break; }
        if(line.length() < 11) { status= TOO_SHORT_STRING;     break; }
        if(line.at(0) != ':')  { status= START_CODE_MISSING;   break; }
        uint dataLen = QByteArray::fromHex(line.mid(1,2)).at(0);
        if(line.length() < int(11 + dataLen*2) ) { 
            status= STRING_LENGHT_MISMATCH;
            break;
        }
        /// Разбор полей параграфа и проверка КС
        QByteArray raw = QByteArray::fromHex(line.mid(1, 10+dataLen*2));
        offset =              (unsigned char)raw.at(1);
        offset = offset*256 + (unsigned char)raw.at(2);
        dataType = (unsigned char)raw.at(3);
        unsigned char ccs = 0;
        for(int i = 0; i < raw.size()-1; i++) {
            ccs += raw.at(i);
        }
        ccs = 1 + ~ccs;
        if(ccs != (unsigned char)raw.at(raw.size()-1)) {
            qDebug()<<"ccs ="<<ccs<<" cs ="<< (unsigned char)raw.at(raw.size()-1);
            status= CHECKSUM_MISMATCH;
            break;
        }
        //qDebug() << "strLen =" << strLen << "offset =" << offset << "dataType =" << dataType << line;
        /// Разбор данных
        switch (dataType) {
        case DataType: // запись содержит данные двоичного файла.
            currOffset = adrPtr & 0xFFFFU;
            if(dataSize == 0) {
                adrPtr += offset;
            } else if(currOffset != offset) {
                qDebug()<<"currOffset ="<<QString::number(currOffset,16)
                       <<" offset ="<<offset;
                status= NEXT_OFFSET_ISNT_CONTINUE_CURRENT_DATA;
                break;
            }
            dest->write(raw.mid(4, dataLen));
            dataSize += dataLen;
            adrPtr += dataLen;
            break;
        case EofType: // запись является концом файла.
            status= EOF_DETECTED;
            break;
        case ExtSegAdr: // запись адреса сегмента (подробнее см. ниже).
            newBase =               (unsigned char)raw.at(4);
            newBase = newBase*256 + (unsigned char)raw.at(5);
            newBase <<= 4;
            if(dataSize != 0) { // если есть данные
                if(newBase != adrPtr) { // и не продолжение данных
                    status= NEXT_ESA_ISNT_CONTINUE_CURRENT_DATA;
                    break;
                }
            } else { // если данных нет
                adrPtr = newBase;
                qDebug()<<"newBase ESA ="<<newBase<<"offset ="<<offset;
            }
            break;
        case StartSegAdr: // Start Segment Address Record
            status= UNSUPPORTED_I16HEX_TYPE;
            break;
        case ExtLinAdr: // запись расширенного адреса 
            newBase =               (unsigned char)raw.at(4);
            newBase = newBase*256 + (unsigned char)raw.at(5);
            newBase <<= 16;
            if(dataSize != 0) { // если есть данные
                if(newBase != adrPtr) { // и не продолжение данных
                    status= NEXT_ELA_ISNT_CONTINUE_CURRENT_DATA;
                    break;
                }
            } else { // если данных нет
                adrPtr = newBase;
                //qDebug()<<"newBase ELA ="<<newBase<<"offset ="<<offset;
                /** http://www.cplusplus.com/reference/cstdio/printf/ */
                qDebug("newBase ELA = %#010X, offset = %#010X", newBase, offset);
            }
            break;
        case StartLinAdr: // Start Linear Address Record.
            newStart =                (unsigned char)raw.at(4);
            newStart = newStart*256 + (unsigned char)raw.at(5);
            newStart = newStart*256 + (unsigned char)raw.at(6);
            newStart = newStart*256 + (unsigned char)raw.at(7);
            (*startAdr) = newStart;
            break;
        default: // не известный тип...
            status= UNKNOWN_RECORD_TYPE;
            break;
        }
    }
    *baseAdr = adrPtr - dataSize;
    src->close();
    dest->close();
    return status;
}

Hex::FileParseError Hex::saveToFile(QString fileName, const QByteArray &data,
                                    quint32 baseAdr, quint32 startAdr)
{
    QFile file(fileName);
    if(fileName.endsWith(".hex", Qt::CaseInsensitive)) {
        return toHex(&file, data, baseAdr, startAdr);
    }
    if(fileName.endsWith(".bin", Qt::CaseInsensitive)) {
        if( !file.open(QIODevice::WriteOnly) ) {
            qDebug()<<file.fileName()<<file.errorString();
            return CANNOT_OPEN_FILE;
        }
        file.write(data);
        file.close();
        return NO_ERROR;
    }
    qDebug()<<"UNSUPPORTED_FILE_FORMAT"<<fileName;
    return UNSUPPORTED_FILE_FORMAT;
}

Hex::FileParseError Hex::saveToFile(QString fileName)
{
    return saveToFile(fileName, d, base, start);
}

Hex::FileParseError Hex::toHex(QIODevice *dest, const QByteArray &data,
                               quint32 baseAdr, quint32 startAdr)
{
    if(baseAdr & 0xF) return ALIGN_BASE_ERROR;
    if(data.size() < 0) return ZERO_SIZE_DATA_ERROR;
    if( !dest->isOpen()) {
        if( !dest->open(QIODevice::WriteOnly) ) {
            QFile *file(qobject_cast<QFile *>(dest));
            if(file) qDebug()<<file->fileName()<<file->errorString();
            return CANNOT_OPEN_FILE;
        } // не забываать закрывать файл
    }
    quint16 offset = baseAdr>>16;
    quint16 adr = baseAdr & 0xFFFF;
    ByteArray d;
    for(int i = 0; i<data.size(); i+=16, adr+=16 ) { // guaranted by ALIGN_BASE_ERROR
        if( (adr == 0) || (i == 0) ) { // в начале файла и при переполнении adr
            d.clear();
            d.append(offset).revers();
            dest->write(doHexString(0, ExtLinAdr, d));
            offset++; // указывает на СЛЕДУЮЩИЙ сегмент
        }
        dest->write(doHexString(adr, DataType, data.mid(i, 16)));
    }
    d.clear();
    d.append(startAdr).revers();
    dest->write(doHexString(0, StartLinAdr, d));
    dest->write(":00000001FF\r\n");
    dest->close();
    qDebug("ok!");
    return NO_ERROR;
}

QByteArray Hex::doHexString(quint16 adr, quint8 recordType, const QByteArray &data)
{
    ByteArray d, r;
    d.append((quint8)data.size());
    r.append((quint16)adr);
    d.append(r.revers().q());
    d.append((quint8)recordType);
    d.append(data);
    unsigned char ccs = 0;
    for(int i = 0; i < d.size(); i++) {
        ccs += d.at(i);
    }
    ccs = 1 + ~ccs;
    d.append((quint8)ccs);
    QByteArray s(":");
    s.append(d.q().toHex().toUpper());
    s.append("\r\n");
    return s;
}
