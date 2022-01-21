#include "sender_data.h"
#include "ui_programmer_dialog.h"
#include "include/log.h"
#include "include/view.h"

SenderData::SenderData(ui_prog* ui, ModelProgrammer* model, ModelLogs* logs, QWidget* viewError, Serializator* serialisator, QObject* parent)
    :inherited(parent)
    , ui(ui)
    , m_model(model)
    , m_logs(logs)
    , m_viewError(viewError)
    , m_serialisator(serialisator)
{
}
SenderData::~SenderData()
{
    delete m_transfer;
    m_transfer = nullptr;
}
void SenderData::logsShow() const noexcept
{
    ui->m_logs->show();
}
QString SenderData::getIP(QModelIndex const& ind, int columnIP) const noexcept
{
    return m_model->data(m_model->index(ind.row(), columnIP, ind), 0).toString();
}
bool SenderData::sendConfiguration()
{
    auto const& ind = mh::selected(ui->m_model);
    auto item = m_model->item(ind.row())->stationItem();
    m_transfer = new Transfer(m_logs, getIP(ind, IP_POSITION), IPS_GLOBAL_PORT_NAME);
    auto sysinfo = m_serialisator->info(item);
    auto iptable = m_serialisator->stations();
    auto keys = m_serialisator->keys(item);
    auto contrlines = m_serialisator->activateControlLines(item);
    m_transfer->loadData(sysinfo, iptable, keys, contrlines);
    m_transfer->send();
    return true;
}
