#pragma once
#include "classes/base_types/station_types.h"
#include "include/delegates/type_delegate.h"
#include "include/delegates/view_delegate.h"
#include "include/delegates/ip_delegate.h"
#include "include/delegates/name_delegate.h"

#define STRING_MAX_LENGTH_NAMESTATIONS 64

template <class U>
class StationsItemInstanceType final {
 public:
    using class_name = StationsItemInstanceType;
    using ui_t       = U;
    using delegate_t = gt::TypeDelegate<StationType, Qt::EditRole>;

 private:
   ui_t* ui = nullptr;

 private:
    delegate_t* instanceDelegate() const noexcept {
        delegate_t::list_t result;
        auto const& list = station_type::defaultStationTypes();
        std::copy(list.cbegin(), list.cend(), std::back_inserter(result));
        return new delegate_t(std::move(result), ui->m_table);
    }
    void instanceWidgets() noexcept {
        ui->m_table->setItemDelegateForColumn(2, instanceDelegate());
        ui->m_table->verticalHeader()->hide();
        ui->m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table->horizontalHeader()->resizeSection(0, 100);
        ui->m_table->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->m_rem->setEnabled(false);
        ui->m_table->horizontalHeader()->setHighlightSections(false);
        ui->m_table->horizontalHeader()->setDefaultSectionSize(0);
        ui->m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        //ui->m_table_groups->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    }
    void instanceDelegates() noexcept {
        ui->m_table->setItemDelegateForColumn(0, new ViewDelegate(4));
        ui->m_table->setItemDelegateForColumn(1, new NameDelegate(STRING_MAX_LENGTH_NAMESTATIONS));
        ui->m_table->setItemDelegateForColumn(3, new IpDelegate());
    }

 public:
    explicit StationsItemInstanceType(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceWidgets();
       instanceDelegates();
    }
};

template <class U>
inline void StationsItemInstance(U* ui) noexcept {
    StationsItemInstanceType<U> instance(ui);
    instance();
}
