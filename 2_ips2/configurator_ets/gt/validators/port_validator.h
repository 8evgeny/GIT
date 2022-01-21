/**
 * \file      ojects/cnf/include/validators/num_validator.h
 * \brief     The Num_validator class provides
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020
 * \version   v.1.1
 * \created   November  (the) 19(th), 2020, 13:48 MSK
 * \updated   November  (the) 19(th), 2020, 21:12 MSK
 * \TODO
**/
#pragma once
#include "strong_int_validator.h"
#include "../convert/integer.h"

/** \namespace gt */
namespace gt {

/**
 * \code
    ui->m_name->setValidator(new gt::PortValidator(ui->m_name));
 * \endcode
 **/
class PortValidator final: public StrongIntValidator {
public:
    using class_name = PortValidator;
    using inherited  = StrongIntValidator;

public:
    static inline auto isValid(QString const& value) noexcept {
        return inherited::isValid(value, 1, 100'001);
    }
    static inline auto isValid(QVariant const& value) noexcept {
        return isValid(value.toString());
    }

public:
    explicit PortValidator(QObject* parent = nullptr) noexcept
        : inherited(1, 100'001, parent) {
    }
};
}  // end namespace gt

