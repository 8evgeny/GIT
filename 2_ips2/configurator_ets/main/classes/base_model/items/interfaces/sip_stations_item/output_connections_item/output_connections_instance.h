#pragma once
#include "include/delegates/view_delegate.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/model_output_connections/model_output_connections.h"

template <class U>
class OutputConnectionsItemInstanceType final {
 public:
    using class_name = OutputConnectionsItemInstanceType;
    using ui_t       = U;
    using model_t    = ModelOutputConnections;

 private:
   ui_t* ui = nullptr;
   model_t* m_model = nullptr;

 private:    
    void instanceWidgets() noexcept {
        ui->m_table_output->setModel(m_model);
        ui->m_table_output->setModel(m_model);
        ui->m_table_output->setShowGrid(false);
        ui->m_table_output->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_output->verticalHeader()->hide();
        ui->m_table_output->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_output->horizontalHeader()->resizeSection(0, 100);
        ui->m_table_output->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_output->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table_output->setItemDelegateForColumn(0, new ViewDelegate(4));
    }

 public:
    explicit OutputConnectionsItemInstanceType(ui_t* ui, model_t* model)
        : ui(ui)
        , m_model(model)
    {
    }
    void operator()() {
       instanceWidgets();
    }
};

template <class U>
inline void outputConnectionsItemInstance(U* ui, ModelOutputConnections* model) noexcept {
    OutputConnectionsItemInstanceType<U> instance(ui, model);
    instance();
}
