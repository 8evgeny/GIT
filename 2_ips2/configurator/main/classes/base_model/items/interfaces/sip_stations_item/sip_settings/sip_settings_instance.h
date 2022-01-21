#pragma once
#include "classes/base_types/station_types.h"
#include "include/validators/string_validator.h"

#define STRING_SIZE 64

template <class U>
class SipStationItemsSettingsInstance final {
 public:
    using class_name = SipStationItemsSettingsInstance;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

 private:
    void instanceWidgets() noexcept {
         ui->m_prefix->setValidator(new StringSizeValidator<STRING_SIZE>(ui->m_prefix));
         ui->m_cid->setValidator(new QIntValidator(0, 1000, ui->m_cid));
         ui->m_login->setValidator(new StringSizeValidator<STRING_SIZE>(ui->m_login));
         ui->m_password->setValidator(new StringSizeValidator<STRING_SIZE>(ui->m_password));         
    }

 public:
    explicit SipStationItemsSettingsInstance(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceWidgets();
    }
};

template <class U>
inline void sipStationItemsSettingsInstance(U* ui) noexcept {
    SipStationItemsSettingsInstance<U> instance(ui);
    instance();
}
