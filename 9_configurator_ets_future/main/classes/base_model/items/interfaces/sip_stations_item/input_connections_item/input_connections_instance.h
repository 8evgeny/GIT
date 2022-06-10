#pragma once
#include "include/delegates/key_delegate.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/model_input_connections/model_input_connections.h"

template <class U>
class  InputConnectionsItemInstanceType final {
 public:
    using class_name = InputConnectionsItemInstanceType;
    using ui_t       = U;
    using model_t    = ModelInputConnections;

 private:
   ui_t* ui = nullptr;
   model_t* m_model = nullptr;

 private:    
    void instanceWidgets() noexcept {
        ui->m_table_input->setModel(m_model);        
        ui->m_table_input->setShowGrid(false);
        ui->m_table_input->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_input->verticalHeader()->hide();
        ui->m_table_input->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_input->horizontalHeader()->resizeSection(0, 100);
        ui->m_table_input->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_input->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table_input->setItemDelegateForColumn(0, new KeyDelegate);
    }

 public:
    explicit InputConnectionsItemInstanceType(ui_t* ui, model_t* model)
        : ui(ui)
        , m_model(model)
    {
    }
    void operator()() {
       instanceWidgets();
    }
};

template <class U>
inline void inputConnectionsItemInstance(U* ui, ModelInputConnections* model) noexcept {
    InputConnectionsItemInstanceType<U> instance(ui, model);
    instance();
}
