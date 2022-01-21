#pragma once
#include "classes/base_types/station_types.h"
#include "include/validators/ip_validator.h"
#include "include/validators/value_int_validator.h"

/* begin namespace empty */
namespace  {

template <class U>
class StationItemsSettingsInstanceCls final {
 public:
    using class_name = StationItemsSettingsInstanceCls;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

 private:
    void instanceWidgets() noexcept {
        ui->m_station_ip->setValidator(new IpValidator(ui->m_station_ip));
        ui->m_station_mask->setValidator(new IpValidator(ui->m_station_mask));
        ui->m_station_gateway->setValidator(new IpValidator(ui->m_station_gateway));
        ui->m_station_cid->setValidator(new ValueIntValidator(ui->m_station_cid));
        station_type::fill(ui->m_station_type);
        ui->m_station_type->setEditable(false);
    }

 public:
    explicit StationItemsSettingsInstanceCls(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceWidgets();
    }
};
} // end empty namespace

template <class U>
inline void StationItemsSettingsInstance(U* ui) noexcept {
    StationItemsSettingsInstanceCls<U> instance(ui);
    instance();
}
