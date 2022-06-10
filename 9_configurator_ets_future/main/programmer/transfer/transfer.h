#pragma once
#include <QDateTime>
#include "include/main_pch.h"
#include "classes/binary_defines.h"
#include "programmer/log_from_socket.h"
#include "include/typedefs.h"

#define LOG_PATH QCoreApplication::applicationDirPath() + IPS_STR("/logs.txt")

class Transfer final : public QObject {
    Q_OBJECT

    friend class TestTransfer;    
    using inherited = QObject;

 public:
    Transfer(ModelLogs* logs, QString const& host, std::uint32_t port, QObject* parent = nullptr);
    ~Transfer();

 public:
    bool send() noexcept;
    bool send(int firstMsg, int secondMsg, QByteArray const& data) noexcept;
    void loadData(QByteArray const& sysinfo, QByteArray const& iptable, QByteArray const& keys, QByteArray const& contrllines) noexcept;

    /**
     * @brief Sync() - синхронизация. каждая заливка конфига начинается с нее (и заканчивается EOT()). Готовность подтверждается принятием Accept()
     * @return true 
     * @return false 
     */
    bool Sync() noexcept;
    /**
     * @brief LoadSysInfo(uint32_t size) - объявление  о  начале передачи системной информации, размера(size). Готовность подтверждается принятием Accept(size)
     * @param size 
     * @return true 
     * @return false 
     */
    bool LoadSysInfo(uint32_t size) noexcept;
    /**
     * @brief LoadIpTable(uint32_t size) - объявление о начале передачи таблиц ассоциаций IP адресов. Готовность подтверждается принятием Accept(size)
     * @param size 
     * @return true 
     * @return false 
     */
    bool LoadIpTable(uint32_t size) noexcept;
    /**
     * @brief LoadKeysTable(uint32_t size) - объявление о начале передачи настроек клавиатуры. Готовность подтверждается принятием Accept(size).
     * @param size 
     * @return true 
     * @return false 
     */
    bool LoadKeysTable(uint32_t size) noexcept;
    /**
     * @brief LoadControllinesTable(uint32_t size) - объявление о начале передачи таблиц ассоциаций IP адресов. Готовность подтверждается принятием Accept(size)
     * @param size 
     * @return true 
     * @return false 
     */
    bool LoadControllinesTable(uint32_t size) noexcept;
    /**
     * @brief ProtoSendData(uint32_t data_size,const QByteArray& data) - трансфер подтвержденных данных
     * @param data_size 
     * @param data 
     * @return true 
     * @return false 
     */
    bool ProtoSendData(uint32_t data_size,const QByteArray& data) noexcept;

signals:
    void errorOcur(const QString &error);

 private:
    bool sendData(std::uint32_t message, std::uint32_t data_size, QByteArray data = {}) noexcept;
    bool waitForResponse() noexcept;
    bool eot() noexcept;

 public:
    QString const& error () const noexcept {
        return m_error;
    }
    QTcpSocket* socket() noexcept {
        return m_socket;
    }

 private:    
    int m_pos = 0;
    QString m_error = {};
    ModelLogs* m_logs = nullptr;
    std::uint8_t* m_buffer = nullptr;
    QTcpSocket* m_socket = nullptr;
    QByteArray m_sysinfo;
    QByteArray m_iptable;
    QByteArray m_keys;
    QByteArray m_contrllines;
};
