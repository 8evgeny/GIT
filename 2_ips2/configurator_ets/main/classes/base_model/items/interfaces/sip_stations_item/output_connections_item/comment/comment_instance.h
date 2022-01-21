#pragma once
#include "classes/base_types/station_types.h"
#include "include/validators/string_validator.h"

#define STRING_SIZE 64

template <class U>
class OutputCommentItemsSettingsInstance final {
 public:
    using class_name = OutputCommentItemsSettingsInstance;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

 private:
    void instanceWidgets() noexcept {         
         ui->m_oid->setValidator(new QIntValidator(ui->m_oid));
         ui->m_name->setValidator(new StringSizeValidator<STRING_SIZE>(ui->m_name));
         ui->m_number->setValidator(new QIntValidator(ui->m_number));
    }

 public:
    explicit OutputCommentItemsSettingsInstance(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceWidgets();
    }
};

template <class U>
inline void outputCommentItemsSettingsInstance(U* ui) noexcept {
    OutputCommentItemsSettingsInstance<U> instance(ui);
    instance();
}
