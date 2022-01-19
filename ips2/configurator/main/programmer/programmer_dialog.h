#pragma once
#include <QObject>
#include "include/typedefs.h"
#include "include/statusbar.h"
#include "programmer/log_from_socket.h"
#include "model_programmer/model_programmer.h"
#include "libips/tools/intercomstation.hpp"
#include "dialogs/message_box/message_box.h"
#include "host_checker.h"

#include "messages/model_tracks/tracks_model.h"

#define MAX_THREAD_COUNT 100
#define IP_POSITION      4

#define IP_SEND          IPS_STR("239.168.10.100")
#define PORT_SEND        13120

IPS_UI(ProgrammerDialog)

class ProgrammerDialog: public QDialog {
    Q_DISABLE_COPY(ProgrammerDialog)

 public:
    using class_name = ProgrammerDialog;
    using ui_prog    = Ui::ProgrammerDialog;
    using inherited  = QDialog;
    using statusBar  = QStatusBar;

 public:
    explicit ProgrammerDialog(StationsItem* stationsItem, QWidget* parent);
    virtual ~ProgrammerDialog() override;

 public:
    bool checkNameStation(QString const& ip, intercomStation const& stationResponce) noexcept;
    void errorMessage(QString const& error) noexcept {
         auto const& err_msg = iMessage::tr("'%1'").arg(error);
         IpsMessageBox::info(m_viewError, err_msg);
    }

 private:
    void setDataForModel(StationsItem* stationsItem, ModelProgrammer* model = nullptr) noexcept;

 private:            
    StationItem* m_item = nullptr;
    ui_prog* ui = nullptr;
    ModelLogs m_logs;
    ModelProgrammer m_model;
    QWidget* m_viewError = nullptr;
    Serializator* m_serilizator = nullptr;
    SenderData* m_senderData = nullptr;
    HostChecker* m_checker = nullptr;
};
