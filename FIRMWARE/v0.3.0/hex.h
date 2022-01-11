#ifndef HEX_H
#define HEX_H

#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QString>
#include <QBuffer>

class Hex
{
public:
    typedef enum {
        EOF_DETECTED,
        NOT_OPENED,
        CANNOT_OPEN_FILE,
        TOO_SHORT_STRING,
        START_CODE_MISSING,
        STRING_LENGHT_MISMATCH,
        CHECKSUM_MISMATCH,
        UNKNOWN_RECORD_TYPE,
        UNSUPPORTED_I16HEX_TYPE,
        NEXT_ESA_ISNT_CONTINUE_CURRENT_DATA,
        NEXT_ELA_ISNT_CONTINUE_CURRENT_DATA,
        NEXT_OFFSET_ISNT_CONTINUE_CURRENT_DATA,
        EOF_ISNT_DETECTED,
        EMPTY_LINE_IS_READED,
        
        ALIGN_BASE_ERROR,
        ZERO_SIZE_DATA_ERROR,
        NO_ERROR, //=========================== 16
        
        NOT_OPENED_SOURCE_FILE,
        NOT_OPENED_DESTINATION_FILE,
        UNSUPPORTED_FILE_FORMAT
    } FileParseError;
    enum RecordType {
        DataType,
        EofType,
        ExtSegAdr,
        StartSegAdr,
        ExtLinAdr,
        StartLinAdr
    };
    enum {
        FORBIDDEN_ADDRESS = 0xFFFFFFFFU,
    };
    explicit Hex();
    quint32 startAdr() { return start; }
    quint32 baseAdr() { return base; }
    quint32 stringNumber() { return stringNum; }
    const QByteArray &data() { return d; }
    int sizeData() { return d.size(); }
    void setData(const QByteArray &bin, const quint32 base, quint32 start) {
        d = bin; this->base = base; this->start = start;
    }

    FileParseError loadFromFile(QString fileName);
    FileParseError lastError() { return error; }
    FileParseError saveToFile(QString fileName);
    static FileParseError fromHex(QIODevice *src, QIODevice *dest,
                                quint32 *baseAdr, quint32 *startAdr, quint32 *stringNumber);
    static FileParseError fromElf(QIODevice *src, QBuffer &dest,
                                  quint32 *baseAdr, quint32 *startAdr);
    static FileParseError saveToFile(QString fileName, const QByteArray &data,
                                     quint32 baseAdr = 0, quint32 startAdr = 0);
    /*static FileParseError saveTo(QIODevice *dest, const QByteArray &data,
                                 quint32 baseAdr = 0, quint32 startAdr = 0);*/
    static FileParseError toHex(QIODevice *dest, const QByteArray &data,
                                quint32 baseAdr = 0, quint32 startAdr = 0);
    static QByteArray doHexString(quint16 adr, quint8 recordType, const QByteArray &data);
    static const QString supportedFormats() {     
        QString supportedFormats;
        supportedFormats.append("All supported (*.hex *.bin *.elf *.axf)");
        supportedFormats.append(";;Intel HEX (*.hex)");
        supportedFormats.append(";;Binary (*.bin)");
        supportedFormats.append(";;Executable and Linkable Format (*.elf *.axf)");
        return supportedFormats;
    }
private:
    quint32 start;
    quint32 base;
    quint32 stringNum;
    QByteArray d;
    FileParseError error;
};

#endif // HEX_H
