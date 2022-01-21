#pragma once
#include "programmer/log_from_socket.h"
#include "model_programmer/model_programmer.h"

#include "messages/model_tracks/tracks_model.h"

template <class U>
class ProgrammerInstance final {
  public:
    using class_name = ProgrammerInstance;
    using ui_t       = U;

   private:
    ui_t* ui = nullptr;
    ModelProgrammer* m_modelProgrammer = nullptr;
    ModelLogs* m_modelLogs = nullptr;

   private:
    void instanceWidgets() noexcept {
         setModels();
         setModelsSettings();
    }
    void setModels() noexcept
    {        
        ui->m_model->setModel(m_modelProgrammer);
        ui->m_logs->setModel(m_modelLogs);
        ui->m_logs->hide();
    }
    void setModelsSettings() noexcept
    {
        ui->m_model->verticalHeader()->hide();
        ui->m_model->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_model->horizontalHeader()->resizeSection(0, 100);
        ui->m_model->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_model->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_model->setShowGrid(false);
        ui->m_model->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->m_model->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->m_model->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
        // ui->m_model->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        //ui->m_model->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    }
   public:
    explicit ProgrammerInstance(ui_t* ui, ModelProgrammer* modelProgrammer = nullptr, ModelLogs* modelLogs = nullptr)
        : ui(ui)
        , m_modelProgrammer(modelProgrammer)
        , m_modelLogs(modelLogs)
    {
    }
    void operator()() {
        instanceWidgets();
    }
};

template <class U>
inline void ProgrammerItemInstance(U* ui, ModelProgrammer* modelProgrammer, ModelLogs* modelLogs) noexcept {
    ProgrammerInstance<U> instance(ui, modelProgrammer, modelLogs);
    instance();
}
