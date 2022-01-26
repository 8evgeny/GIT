#include "host_checker.h"
#include "ui_programmer_dialog.h"
#include "programmer/client_server/server/islp_server.h"
#include "dialogs/send_configuration/send_configuration.h"

HostChecker::HostChecker(ui_prog* ui, ModelProgrammer* model, ModelLogs* logs, SenderData* senderData, QObject* parent)
    :inherited(parent)
    , ui(ui)
    , m_model(model)
    , m_logs(logs)
    , m_senderData(senderData)
{    
    isConnectedToHost(IP_SEND, PORT_SEND);    
}
HostChecker::~HostChecker()
{
    delete m_transfer;
    m_transfer = nullptr;
}
void HostChecker::disconnectSocket() noexcept
{
    m_transfer->socket()->disconnect();
    m_transfer->socket()->close();
}
void HostChecker::setButtonWriteConfig(int itr, bool status) noexcept
{
    QModelIndex ind = m_model->index(itr, 0, {});
    QModelIndex indButton = m_model->index(ind.row(), 6, ind);
    auto obj = new QPushButton(ui->m_model);
    obj->setText(iInfo::tr("Write"));
    ui->m_model->setIndexWidget(indButton, obj);    
    if (status)
        instanceSignals(obj);
}
QString HostChecker::getStation(int itr) noexcept
{
    QModelIndex ind = m_model->index(itr, 0, {});
    QModelIndex indIP = m_model->index(ind.row(), IP_POSITION, ind);
    return m_model->data(indIP, Qt::DisplayRole).toString();
}
void HostChecker::setStatusStation(int itr) noexcept
{    
    QModelIndex index = m_model->index(itr, IP_STATUS_POSITION);
    m_model->setData(index, false, ModelProgrammer::StatusRole);
}
bool HostChecker::checkNameStation(QString const& ip, intercomStation const& stationResponce) noexcept
{ 
    QString ipAddr;
    ipAddr = QString::number(static_cast<int>(stationResponce.ipaddr[0]))  + IPS_STR(".")
            + QString::number(static_cast<int>(stationResponce.ipaddr[1])) + IPS_STR(".")
            + QString::number(static_cast<int>(stationResponce.ipaddr[2])) + IPS_STR(".")
            + QString::number(static_cast<int>(stationResponce.ipaddr[3]));
    return ipAddr == ip;
}
void HostChecker::checkableHosts(bool)
{    
    std::list<intercomStation> stationsResponce;
    IslpServer server;
    server.start();
    stationsResponce = server.stations();
    if (!stationsResponce.size()) {
        for (auto i=0; i<m_model->items().size(); ++i) {
            setStatusStation(i);
            setButtonWriteConfig(i, true);
        }
    }
    else {
         LOG_ERROR(!stationsResponce.empty(), "stationsResponce must be is not empty!");
         std::list<intercomStation>::iterator it = stationsResponce.begin();
         for (auto i=0; i<m_model->items().size(); ++i) {
             setStatusStation(i);
             setButtonWriteConfig(i, true);
             QModelIndex index = m_model->index(i, IP_STATUS_POSITION);
             if (checkNameStation(getStation(i), *it))
                 m_model->setData(index, true, ModelProgrammer::StatusRole);

             if (i < static_cast<int>(stationsResponce.size()-1))
                 ++it;
        }
    }
}
void HostChecker::instanceSignals(QPushButton* button)
{
    QObject::connect(button, &QPushButton::pressed, this, [button, this] () {
        button->setText(iInfo::tr("Stop"));
        if (m_senderData->sendConfiguration()) {
            SendConfigurationDialog dlg;
            dlg.exec();
        }
    });
}
void HostChecker::isConnectedToHost(QString const& host, int port) noexcept
{
    m_transfer = new Transfer(m_logs, host, static_cast<std::uint32_t>(port));
    QObject::connect(m_transfer->socket(), SIGNAL(connected()), this, SLOT(checkableHosts()));
    QObject::connect(m_transfer->socket(), &QTcpSocket::disconnected, this, [this] () {
        for (auto i=0; i<m_model->items().size(); ++i)
            setStatusStation(i);
    });
}
