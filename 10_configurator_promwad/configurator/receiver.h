#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>

class QUdpSocket;

/**
* The Receiver class handles the processing of the incoming UDP multicast datagrams.
* It uses a QUdpSocket to retrieve the multicast datagrams and makes the content of
* the multicast datagram available to the UI via the 'status' property.
*/
class Receiver : public QObject
{
    Q_OBJECT

    // The property that makes the content of a received multicast datagram available to the UI
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    Receiver(QObject *parent = nullptr);

Q_SIGNALS:
    // The change notification signal of the status property
    void statusChanged();
    void statusChangedJson(QString idJson, QString versionJson, QString macJson, QString ipJson);
    void statusFirmwareLoad(QString idDevice);
    void statusConfigurationLoad(QString idDevice);

private Q_SLOTS:

public slots:
    //set network card
    void setCard(const QString &card);

    // This method is called whenever a new multicast datagram arrived on the socket
    void processPendingDatagrams();


private:
    // The accessor method of the status property
    QString status() const;

    // The UDP socket object
    QUdpSocket *m_udpSocket;

    // The multicast address to listen on
    QHostAddress m_groupAddress;

    // The content of the received multicast datagram
    QString m_status;

    const qint32 MIN_ID = 100; /*! Minimum station number */
    const quint16 PORT_JSON = 65215; /*! Port for processing incoming requests */
};

#endif
