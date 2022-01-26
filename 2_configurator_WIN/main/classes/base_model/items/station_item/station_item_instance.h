#pragma once
#include "include/delegates/cidgt_line_edit_delegate.h"
#include "station_item_settings/citgit_helper.h"
#include "include/delegates/priority_delegate.h"
#include "include/delegates/key_delegate.h"
#include "include/delegates/name_delegate.h"
#include "main/classes/base_model/items/station_item/model_keys/model_keys.h"

#define STRING_MAX_LENGTH_NAMEKEYS 64
template <class U>
class StationItemInstanceType final {
 public:
    using class_name = StationItemInstanceType;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

   void instanceWidgets() noexcept {
        ui->m_table->verticalHeader()->hide();
        ui->m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table->horizontalHeader()->resizeSection(0, 100);
        ui->m_table->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_table->setEditTriggers(QAbstractItemView::DoubleClicked);
        ui->m_rem->setEnabled(false);
   }
   void instancePriorityDelegate() noexcept {
        ui->m_table->setItemDelegateForColumn(4, new PriorityDelegate);
   }
   void instanceDelegateKeyName() noexcept {
        auto delegate = new NameDelegate(STRING_MAX_LENGTH_NAMEKEYS);
        ui->m_table->setItemDelegateForColumn(1, delegate);        
        ui->m_table->setItemDelegateForColumn(0, new KeyDelegate);
   }

 public:
    explicit StationItemInstanceType(ui_t* ui)
        : ui(ui)       
    {
    }
    void operator()() {
       instanceWidgets();
       instancePriorityDelegate();
       instanceDelegateKeyName();
    }
};

template <class U>
inline void StationItemInstance(U* ui) noexcept {
    StationItemInstanceType<U> instance(ui);
    instance();
}
