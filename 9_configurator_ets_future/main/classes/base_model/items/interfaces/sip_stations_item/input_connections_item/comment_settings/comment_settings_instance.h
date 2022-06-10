#pragma once
#include "include/validators/string_validator.h"
#include "main/classes/base_types/function_types.h"

#define STRING_SIZE 64

template <class U>
class CommentItemsSettingsInstance final {
 public:
    using class_name = CommentItemsSettingsInstance;
    using ui_t       = U;

 private:
   ui_t* ui = nullptr;

 private:
    void instanceWidgets() noexcept {
        function_type::fill(ui->m_function);
        ui->m_destination->setValidator(new StringSizeValidator<STRING_SIZE>(ui->m_destination));
        ui->m_priority->setValidator(new QIntValidator(1, 100, ui->m_priority));
        ui->m_input_number->setValidator(new QIntValidator(1, 100, ui->m_input_number));
    }

 public:
    explicit CommentItemsSettingsInstance(ui_t* ui)
        : ui(ui) {
    }
    void operator()() {
       instanceWidgets();
    }
};

template <class U>
inline void commentItemsSettingsInstance(U* ui) noexcept {
    CommentItemsSettingsInstance<U> instance(ui);
    instance();
}
