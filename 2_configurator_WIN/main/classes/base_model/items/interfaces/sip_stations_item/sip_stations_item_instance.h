#pragma once
#include "model_destination/model_destination.h"
#include "model_rules/model_rules.h"
#include "include/delegates/view_delegate.h"
#include "include/delegates/name_delegate.h"
#include "include/delegates/ip_delegate.h"
#include "include/delegates/type_delegate.h"
#include <gt/delegate/combobox_delegate.h>
#include "classes/base_types/protocol_types.h"
#include "classes/base_types/registration_types.h"
#include "classes/base_model/items/interfaces/sip_stations_item/sip_stations_item.h"
#include "main/classes/base_model/items/interfaces/sip_stations_item/model_destination/destination.h"
#include "classes/app.h"

#define STRING_MAX_LENGTH_NAMESTATIONS 64

template <class U>
class SipStationsItemInstanceType final {
 public:
    using class_name = SipStationsItemInstanceType;
    using ui_t       = U;
    using model_t    = ModelDestination;
    using model_r    = ModelRules;
    using delegate_p = gt::TypeDelegate<ProtocolType, Qt::EditRole>;
    using delegate_r = gt::TypeDelegate<RegistrationType, Qt::EditRole>;

 private:
   ui_t* ui = nullptr;
   model_t* m_modelDest = nullptr;
   model_r* m_modelRules = nullptr;

 private:
   delegate_p* instanceProtocolDelegate() const noexcept {
       delegate_p::list_t result;
       auto const& list = protocol_type::defaultProtocolTypes();
       std::copy(list.cbegin(), list.cend(), std::back_inserter(result));
       return new delegate_p(std::move(result), ui->m_table_destiations);
   }
   delegate_r* instanceRegistrationDelegate() const noexcept {
       delegate_r::list_t result;
       auto const& list = registration_type::defaultRegistrationTypes();
       std::copy(list.cbegin(), list.cend(), std::back_inserter(result));
       return new delegate_r(std::move(result), ui->m_table_destiations);
   }

private:
    void instanceWidgets() noexcept {       
        ui->m_table_rules_routings->setModel(m_modelRules);
        ui->m_table_rules_routings->setShowGrid(false);        
        ui->m_table_rules_routings->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_rules_routings->verticalHeader()->hide();
        ui->m_table_rules_routings->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_rules_routings->horizontalHeader()->resizeSection(0, 100);
        ui->m_table_rules_routings->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_rules_routings->setSelectionBehavior(QAbstractItemView::SelectRows);        
        ui->m_rem_rules->setEnabled(m_modelRules->rowCount());

        ui->m_table_destiations->setModel(m_modelDest);
        ui->m_table_destiations->setShowGrid(false);        
        ui->m_table_destiations->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_destiations->verticalHeader()->hide();
        ui->m_table_destiations->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->m_table_destiations->horizontalHeader()->resizeSection(0, 100);
        ui->m_table_destiations->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->m_table_destiations->setSelectionBehavior(QAbstractItemView::SelectRows);      
        ui->m_rem->setEnabled(m_modelDest->rowCount());        
    }
    void instanceDelegates() noexcept {
        ui->m_table_rules_routings->setItemDelegateForColumn(0, new ViewDelegate(4));
        ui->m_table_destiations->setItemDelegateForColumn(1, new IpDelegate());
        ui->m_table_destiations->setItemDelegateForColumn(0, new NameDelegate(STRING_MAX_LENGTH_NAMESTATIONS));
        ui->m_table_destiations->setItemDelegateForColumn(2, instanceProtocolDelegate());
        ui->m_table_destiations->setItemDelegateForColumn(3, instanceRegistrationDelegate());
    }

 public:
    explicit SipStationsItemInstanceType(ui_t* ui, model_t* modelDest, model_r* modelRules)
        : ui(ui)
        , m_modelDest(modelDest)
        , m_modelRules(modelRules)

    {
    }
    void operator()() {
       instanceWidgets();
       instanceDelegates();       
    }
};

template <class U>
inline void sipStationsItemInstance(U* ui, ModelDestination* modelDest, ModelRules* modelRules) noexcept {
    SipStationsItemInstanceType<U> instance(ui, modelDest, modelRules);
    instance();
}
