#pragma once
#include <QObject>
#include <QByteArray>
#include "log_from_socket.h"
#include "transfer/transfer.h"
#include "programmer/log_from_socket.h"
#include "dialogs/message_box/message_box.h"
#include "model_programmer/model_programmer.h"
#include "serializator.h"

#define MAX_THREAD_COUNT 100
#define IP_POSITION      4

#define IP_SEND         IPS_STR("239.168.10.100")
#define PORT_SEND       13120

IPS_UI(ProgrammerDialog)

class SenderData final : public QObject
{
    Q_OBJECT

public:
   using class_name = SenderData;
   using inherited  = QObject;
   using ui_prog    = Ui::ProgrammerDialog;

public:
    SenderData(ui_prog* ui,
               ModelProgrammer* model,
               ModelLogs* logs,
               QWidget* viewError,
               Serializator* serialisator,
               QObject* parent = nullptr);
    ~SenderData();

public:
    bool sendConfiguration();
    void logsShow() const noexcept;
    QString getIP(QModelIndex const& ind, int columnIP) const noexcept;
    void errorMessage(QString const& error) noexcept {
         auto const& err_msg = iMessage::tr("Error occurred while sending configuration: '%1'").arg(error);
         m_logs->setError(QDateTime::currentDateTime().toString(IPS_STR("[dd.MM.yyyy hh.mm.ss]")) + IPS_STR("  ") + err_msg);
         m_logs->insertRows(0, 1);
         IpsMessageBox::error(m_viewError, err_msg);
   }

private:
    ui_prog* ui = nullptr;
    ModelProgrammer* m_model = nullptr;
    ModelLogs* m_logs;    
    QWidget* m_viewError = nullptr;
    Transfer* m_transfer = nullptr;
    Serializator* m_serialisator = nullptr;
};
