#include "receiver.h"

#include <QtNetwork/QUdpSocket>
#include <QtNetwork>

#include "Lib/qjson/include/QJson/Parser"
#include "Lib/qjson/include/QJson/Serializer"
#include "Lib/qjson/src/cmdlineparser.h"
#include "Lib/qjson/src/qjson_debug.h"


Receiver::Receiver(QObject *parent)
    : QObject(parent)
{
    //Initialize the multicast address
    m_groupAddress = QHostAddress("232.0.0.0");

    /**
    * Initialize the 'status' property with some data, it will
    * be shown in the UI until the first datagram arrives.
    */
    m_status = tr("Listening for multicasted messages");

    // Create a new UDP socket and bind it against port 99
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress(QHostAddress::AnyIPv4), PORT_JSON, QUdpSocket::ShareAddress);

    // Tell the UDP socket which multicast group it should join
    m_udpSocket->joinMulticastGroup(m_groupAddress);

    /**
    * Create signal/slot connection to invoke processPendingDatagrams() whenever
    * a new multicast datagram is received by the socket.
    */
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Receiver::setCard(const QString &card)
{
    disconnect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    m_udpSocket->disconnectFromHost();
    delete m_udpSocket;

    //Initialize the multicast address
    m_groupAddress = QHostAddress("232.0.0.0");

    /**
    * Initialize the 'status' property with some data, it will
    * be shown in the UI until the first datagram arrives.
    */
    m_status = tr("Listening for multicasted messages");

    // Create a new UDP socket and bind it against port 99
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress(QHostAddress::AnyIPv4), PORT_JSON, QUdpSocket::ShareAddress);
    m_udpSocket->setMulticastInterface(QNetworkInterface::interfaceFromName(card));

    // Tell the UDP socket which multicast group it should join
    m_udpSocket->joinMulticastGroup(m_groupAddress, QNetworkInterface::interfaceFromName(card));

    /**
    * Create signal/slot connection to invoke processPendingDatagrams() whenever
    * a new multicast datagram is received by the socket.
    */
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Receiver::processPendingDatagrams()
{
    // Now read all available datagrams from the socket
    while (m_udpSocket->hasPendingDatagrams()) {
        // Create a temporary buffer ...
        QByteArray datagram;

        // ... with the size of the received multicast datagram ...
        datagram.resize(m_udpSocket->pendingDatagramSize());

        // ... and copy over the received multicast datagram into that buffer.
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        // Update the 'status' property with the content of the received multicast datagram
        m_status = tr("Received datagram: \"%1\"").arg(datagram.data());
        qDebug() << datagram.data();

        QJson::Parser parser;
        bool ok;

        QVariantMap result = parser.parse(datagram.data(), &ok).toMap();

        if (ok) {
            qint32 idTmp = result["ID"].toInt();
            qDebug() << "ID:" << idTmp;
            QString id = QString::number(idTmp);

            qint32 ver = result["Version"].toInt();
            qDebug() << "Version:" << ver;
            qint32 sub = result["Sub"].toInt();
            qDebug() << "Sub:" << sub;
            QString version = QString::number(ver) +  QString(".") + QString::number(sub);

            QString ip = QString(result["IP"].toString());
            qDebug() << "IP:" << ip;

            QString mac = "";
            foreach (QVariant plugin, result["MAC"].toList()) {
                qint32 tmp = plugin.toInt();
                qDebug() << " " << tmp;
                QString valueInHex = QString("%1").arg(tmp, 0, 16);
                mac += valueInHex;
            }

            if (id.toInt() >= MIN_ID)
                emit statusChangedJson(id, version, mac, ip);

            qint32 writeConfigId = result["writeConfigId"].toInt();
            qint32 writeFirmwareId = result["writeFirmwareId"].toInt();
            QString status = QString(result["status"].toString());

            if ((writeConfigId >= MIN_ID) && (status == QString("ok"))) {
                emit statusConfigurationLoad(QString::number(writeConfigId));
            }

            if ((writeFirmwareId >= MIN_ID) && (status == QString("ok"))) {
                emit statusFirmwareLoad(QString::number(writeFirmwareId));
            }

        }
        emit statusChanged();

    }
}

QString Receiver::status() const
{
    return m_status;
}
