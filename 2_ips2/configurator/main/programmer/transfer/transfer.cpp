#include "include/tr.h"
#include "transfer.h"
#include <QTcpSocket>
#include "include/log.h"
#include "classes/binary_defines.h"
#include "include/error_defines.h"
#include "include/error_messages.h"

#include <QFile>
#include <QMessageLogger>

Transfer::Transfer(ModelLogs* logs, QString const& host, std::uint32_t port, QObject* parent)
    : inherited(parent)
    , m_logs(logs)
    , m_buffer(new (uint8_t[FTRANSFER_MAX_PACKET_SIZE]))
    , m_socket(new QTcpSocket())
{
    m_socket->connectToHost(host, static_cast<quint16>(port));
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QObject::connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [this] (auto err) {
#else
    QObject::connect(m_socket, &QAbstractSocket::errorOccurred, [this] (auto err) {
#endif
        QString msg = error_messages::create(IPS_STR("Unknown error"), IPS_ERROR_UNKNOWN, m_socket->errorString());
        if (err == QAbstractSocket::NetworkError) {
            msg = error_messages::create(IPS_STR("Network error"), IPS_ERROR_NETWORK, m_socket->errorString());            
            ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(msg);
            LOG_ERROR(false, "Server returned 'Network error' message");
            ++m_pos;
        }
        else if (err == QAbstractSocket::QAbstractSocket::SocketTimeoutError) {
            msg = error_messages::create(IPS_STR("Timeout error"), IPS_ERROR_TIMEOUT, m_socket->errorString());
            ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(msg);
            LOG_WARNING(false, "Server returned 'Timeout error' message");
            ++m_pos;
        }
        else if (err == QAbstractSocket::QAbstractSocket::TemporaryError) {
            msg = error_messages::create(IPS_STR("Temporary error"), IPS_ERROR_TEMPORARY, m_socket->errorString());
            ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(msg);
            LOG_ERROR(false, "Server returned 'Temporary error' message");
            ++m_pos;
        }
        emit(errorOcur(msg));
        m_error = msg;
    });
}
Transfer::~Transfer()
{
    delete[] m_buffer;
    if(m_socket->isValid() && m_socket->isOpen())
       m_socket->disconnectFromHost();
    delete m_socket;
}
bool Transfer::sendData(std::uint32_t message, std::uint32_t data_size, QByteArray data) noexcept
{
    std::uint32_t header[2];
    header[0] = message;
    header[1] = data_size;

    QByteArray send(reinterpret_cast<char*>(&header), sizeof(header));
    if (!data.isEmpty())
        send.append(data);

    auto size = m_socket->write(send);
    if (static_cast<std::uint32_t>(size) != static_cast<std::uint32_t>(send.size())) {
        emit(errorOcur(IPS_STR("Logic error(1)")));
        m_error = IPS_STR("Logic error(1)");
        ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(m_error);
        LOG_INFO(static_cast<std::uint32_t>(size) == static_cast<std::uint32_t>(send.size()), "Size of write data(%1) is not equal to sizeof(ips_load)=%2", static_cast<std::uint32_t>(size), static_cast<std::uint32_t>(send.size()));
        ++m_pos;
        return false;
    }

    #if _DEBUG
       auto result = m_socket->waitForBytesWritten(30000);
    #else
      auto result = m_socket->waitForBytesWritten(1000);
    #endif
    if (!result) {
       m_logs->setError(QDateTime::currentDateTime().toString(IPS_STR("[dd.MM.yyyy hh.mm.ss]")) + IPS_STR("  ") + iDesc::tr("Time waiting is done"));
       m_logs->insertRows(0,1);
       ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(IPS_STR("Time waiting is done"));
       LOG_INFO(!result, "Time waiting is done", static_cast<std::uint32_t>(size), static_cast<std::uint32_t>(send.size()));
       ++m_pos;
    }
   return result;
}
bool Transfer::eot() noexcept
{
    auto eotResponse = IPS_BSP_CONF_CMD_EOT;
    if ((!m_socket->write(reinterpret_cast<char*>(&eotResponse),sizeof(eotResponse)))
       || (!m_socket->waitForBytesWritten()))
        return false;
    return true;
}
bool Transfer::waitForResponse() noexcept
{
    if(!m_socket->waitForReadyRead(IPS_GLOBAL_TIME_OUT_DEBUG))
        return false;

    auto const length = m_socket->read(reinterpret_cast<char*>(m_buffer), FTRANSFER_MAX_PACKET_SIZE);
    if (length == 0 || length > static_cast<int>(sizeof(std::uint32_t)*2)) {
        emit(errorOcur(IPS_STR("Response size mismatch")));
        m_error = IPS_STR("Response size mismatch");
        ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(m_error);
        ++m_pos;
        return false;
    }
    if (*reinterpret_cast<std::uint32_t*>(m_buffer) == IPS_BSP_RESPONSE_ACCEPT) {
        return true;
    }
    return false;
}
bool Transfer::send(int firstMsg, int secondMsg, QByteArray const& data) noexcept
{
    auto const size = static_cast<std::uint32_t>(data.length());
    if (!sendData(static_cast<std::uint32_t>(firstMsg), size)) {
        m_logs->setError(QDateTime::currentDateTime().toString(IPS_STR("[dd.MM.yyyy hh.mm.ss]")) + IPS_STR("  ") + iDesc::tr("LOADIPTABLE was not send to server"));
        m_logs->insertRows(0,1);
        LOG_INFO(false, "LOADIPTABLE was not send to server");
        ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(IPS_STR("LOADIPTABLE was not send to server"));
        ++m_pos;
        return false;
    }

    if (!waitForResponse()) {
        m_logs->setError(QDateTime::currentDateTime().toString(IPS_STR("[dd.MM.yyyy hh.mm.ss]")) + IPS_STR("  ") + iDesc::tr("Timeout is done"));
        m_logs->insertRows(0,1);
        LOG_INFO(false, "Timeout is done");
        ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(IPS_STR("Timeout is done"));
        ++m_pos;
        return false;
    }

    if (!sendData(static_cast<std::uint32_t>(secondMsg), size, data)) {
        m_logs->setError(QDateTime::currentDateTime().toString(IPS_STR("[dd.MM.yyyy hh.mm.ss]")) + IPS_STR("  ") + iDesc::tr("DATASEND was not send to server"));
        m_logs->insertRows(0,1);
        LOG_INFO(false, "DATASEND was not send to server");
        ProgramFileLog(LOG_PATH, __FILE__, __FUNCTION__, static_cast<uint>(m_pos)).write(IPS_STR("DATASEND was not send to server"));
        ++m_pos;
        return false;
    }

    return waitForResponse()
        && eot();
}
/**
 * @details description see in README.md 
 * 
 */
    /**
     * @brief Sync() - синхронизация. каждая заливка конфига начинается с нее (и заканчивается EOT()). Готовность подтверждается принятием Accept()
     */
    bool Transfer::Sync() noexcept
    {
        uint32_t syncResponce = IPS_BSP_CONF_CMD_SYNC;
        if ((!m_socket->write(reinterpret_cast<char*>(&syncResponce),sizeof(syncResponce)))
            || (!m_socket->waitForBytesWritten())) return false;
        return true;
    }
    /**
     * @brief LoadSysInfo(uint32_t size) - объявление  о  начале передачи системной информации, размера(size). Готовность подтверждается принятием Accept(size)
     */
    bool Transfer::LoadSysInfo(uint32_t size) noexcept
    {
        return sendData(IPS_BSP_CONF_CMD_LOADSYSINFO,size);
    }
    /**
     * @brief LoadIpTable(uint32_t size) - объявление о начале передачи таблиц ассоциаций IP адресов. Готовность подтверждается принятием Accept(size)
     */
    bool Transfer::LoadIpTable(uint32_t size) noexcept
    {
        return sendData(IPS_BSP_CONF_CMD_LOADIPTABLE,size);
    }
    /**
     * @brief LoadKeysTable(uint32_t size) - объявление о начале передачи настроек клавиатуры. Готовность подтверждается принятием Accept(size).
     */
    bool Transfer::LoadKeysTable(uint32_t size) noexcept
    {
        return sendData(IPS_BSP_CONF_CMD_LOADKEYS,size);
    }
    /**
     * @brief LoadControllinesTable(uint32_t size) - объявление о начале передачи настройки линий управления. Готовность подтверждается принятием Accept(size) 
     */
    bool Transfer::LoadControllinesTable(uint32_t size) noexcept
    {
        return sendData(IPS_BSP_CONF_CMD_LOADCONTROLS,size);
    }
    /**
     * @brief ProtoSendData(uint32_t data_size,const QByteArray& data) - трансфер подтвержденных данных 
     */
    bool Transfer::ProtoSendData(uint32_t data_size,const QByteArray& data) noexcept
    {
        return sendData(IPS_BSP_CONF_CMD_DATASEND,data_size,data);
    }
void Transfer::loadData(QByteArray const& sysinfo, QByteArray const& iptable, QByteArray const& keys, QByteArray const& contrllines) noexcept
{
    m_sysinfo = sysinfo;
    m_iptable = iptable;
    m_keys = keys;
    m_contrllines = contrllines;
}
bool Transfer::send() noexcept
{
    if(!Sync())return false;
    if(!waitForResponse())return false;

    if(!LoadSysInfo(static_cast<uint32_t>(m_sysinfo.size())))return false;
    if(!waitForResponse())return false;
    if(!ProtoSendData(static_cast<uint32_t>(m_sysinfo.size()),m_sysinfo))return false;
    if(!waitForResponse())return false;

    if(!LoadIpTable(static_cast<uint32_t>(m_iptable.size())))return false;
    if(!waitForResponse())return false;
    if(!ProtoSendData(static_cast<uint32_t>(m_iptable.size()),m_iptable))return false;
    if(!waitForResponse())return false;

    if(!LoadKeysTable(static_cast<uint32_t>(m_keys.size())))return false;
    if(!waitForResponse())return false;
    if(!ProtoSendData(static_cast<uint32_t>(m_keys.size()),m_keys))return false;
    if(!waitForResponse())return false;

    if(!LoadControllinesTable(static_cast<uint32_t>(m_contrllines.size())))return false;
    if(!waitForResponse())return false;
    if(!ProtoSendData(static_cast<uint32_t>(m_contrllines.size()),m_contrllines))return false;
    if(!waitForResponse())return false;

    return eot();
}
