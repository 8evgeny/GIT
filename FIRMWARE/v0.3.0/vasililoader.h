#ifndef VASILILOADER_H
#define VASILILOADER_H

#include "hex.h"
#include "mdruartloader.h"

#include <QObject>
#include <QSerialPort>
#include <QFlags>
#include <QString>
#include <QMetaType>

class VasiliLoaderClient : public QObject
{
    Q_OBJECT
    
public:
    enum WorkTypeFlag {
        NoWork =  0,
        Dump =    1,
        Erase =   2,
        Program = 4,
        Verify =  8,
        Run =    16
    };
    Q_DECLARE_FLAGS(WorkType, WorkTypeFlag)
    //Q_FLAG(WorkType)
    
    explicit VasiliLoaderClient(QObject *parent = nullptr);
    ~VasiliLoaderClient();
    void setBaseAddress(uint32_t a) { m_baseAddress = a; }
    void setFirmwareFilename(QString filename) { fwfn = filename; }
    void setFlasherFilename(QString filename) { flasherfn = filename; }
    void setDumpFilename(QString filename) { dumpfn = filename; }
    void setPortName(QString portname) { this->portname = portname; }
    void setBaudRate(QSerialPort::BaudRate baudrate){this->baudrate = baudrate;}
    void setMcuType(Mcu::Type type) { mcuType = type; }
    
signals:
    void dumpFilenameChanged(QString dumpfn);
    void statusChanged(QString s);
    void textChanged(QString s);
    void stopButtonSetEnable(bool);
    void workFinished();
    
public slots:
    //bool doWork(WorkType work);
    void stopWork() { stop = true; }
    bool doWorkNew(VasiliLoaderClient::WorkType w);

private:
    bool loadFirmware();
    bool loadFlasher();
    QByteArray portWR(const QByteArray &send, int bytesCount, int timeout);
    bool checkAndLoadFiles();
    int tryConnect(Mcu::Type& dev, int& rev); // MdrUartLoaderClient::ErrorCode
    bool dump();
    bool erase();
    bool program();
    bool verify();
    bool runFW();
		
		QByteArray getIdStr();
		bool checkIdStr();
		bool checkConnectNew();
		uint32_t getFlasherStartAddress();
		
    Hex *flasher, *firmware; // TODO: создавать и удалять их динамически, как dumpFile
    QSerialPort *port;
    MdrUartLoaderClient *mulc;
    WorkType work;
    bool stop;
    //QFile *dumpFile;
    QString fwfn, dumpfn, flasherfn, portname;
    QSerialPort::BaudRate baudrate;
    Mcu::Type mcuType;
    uint32_t m_baseAddress;
};

#endif // VASILILOADER_H
