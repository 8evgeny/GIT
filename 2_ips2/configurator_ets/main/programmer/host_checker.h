#pragma once
#include <QObject>
#include <QString>
#include <QThreadPool>
#include <QHostAddress>
#include "libips/tools/intercomstation.hpp"
#include "model_programmer/model_programmer.h"
#include "sender_data.h"
#include "transfer/test_transfer.h"
#include "log_from_socket.h"

#define IP_POSITION 4
#define IP_SEND     IPS_STR("239.168.10.100")
#define PORT_SEND   13120

IPS_UI(ProgrammerDialog)

class HostChecker final : public QObject
{
    Q_OBJECT

    using class_name = HostChecker;
    using inherited  = QObject;
    using ui_prog    = Ui::ProgrammerDialog;

private slots:
    void checkableHosts(bool is = false);

public:
    void disconnectSocket() noexcept;
    QString getStation(int itr) noexcept;
    void setStatusStation(int itr) noexcept;
    void instanceSignals(QPushButton* button = nullptr);
    void setButtonWriteConfig(int itr, bool status) noexcept;
    void isConnectedToHost(QString const& host, int port) noexcept;
    bool checkNameStation(QString const& ip, intercomStation const& stationResponce) noexcept;    

public:
    HostChecker() = default;
    HostChecker(ui_prog* ui, ModelProgrammer* model, ModelLogs* logs, SenderData* senderData, QObject* parent = nullptr);
    ~HostChecker();

private:
    ui_prog* ui = nullptr;
    ModelProgrammer* m_model = nullptr;
    ModelLogs* m_logs = nullptr;
    SenderData* m_senderData = nullptr;
    Transfer* m_transfer = nullptr;
};
