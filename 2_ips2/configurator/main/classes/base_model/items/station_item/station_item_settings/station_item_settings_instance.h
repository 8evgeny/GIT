#pragma once
#include "citgit_helper.h"
#include "include/validators/ip_validator.h"
#include "include/validators/duration_validator.h"
#include "main/classes/base_types/station_types.h"
#include "main/classes/base_types/function_types.h"

template <class U>
class StationItemSettingsInstanceCls final {
 public:
    using class_name = StationItemSettingsInstanceCls;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

 private:
    void instanceStationSettingsWidgets() noexcept {
        ui->m_station_ip->setValidator(new IpValidator(ui->m_station_ip));
        ui->m_station_mask->setValidator(new IpValidator(ui->m_station_mask));
        ui->m_station_gateway->setValidator(new IpValidator(ui->m_station_gateway));
        ui->m_station_cid->setValidator(new QIntValidator(ui->m_station_cid));
        station_type::fill(ui->m_station_type);
        ui->m_station_type->setEditable(false);
    }
    void instanceKeySettingsWidgets() noexcept {
        ui->m_key_dest->setEditable(true);
        ui->m_key_priority->setValidator(new QIntValidator(1, 100, ui->m_key_priority));
        function_type::fill(ui->m_key_func);
        ui->m_key_pretone_play->setValidator(new QIntValidator(1, 100, ui->m_key_pretone_play));
        ui->m_key_dest->addItems(std::move(cidList()));
        ui->m_key_pretone_duration->setValidator(new gt::validator::Duration(ui->m_key_pretone_duration));
    }

 public:
    explicit StationItemSettingsInstanceCls(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceStationSettingsWidgets();
       instanceKeySettingsWidgets();
    }
};

template <class U>
inline void StationItemSettingsInstance(U* ui) noexcept {
    StationItemSettingsInstanceCls<U> instance(ui);
    instance();
}
