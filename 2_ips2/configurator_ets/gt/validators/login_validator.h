/**
 * \file      ojects/cnf/include/validators/login_validator.h 
 * \brief     The Login_validator class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.2
 * \created   November  (the) 06(th), 2020, 13:15 MSK
 * \updated   November  (the) 06(th), 2020, 14:43 MSK
 * \TODO      
**/
#pragma once
#include <QValidator>

/** \namespace gt */
namespace gt {

/**
 * \code
    ui->m_login->setValidator(new gt::LoginValidator(ui->m_login));
 * \endcode
 **/
class LoginValidator final: public QValidator {
public:
    using class_name = LoginValidator;
    using inherited  = QValidator;

public:
    static inline auto isValid(QString const& value) noexcept {
        if(!value.isEmpty() && value[0].isDigit())
            return false;
        for(auto const ch: value)
            if(ch.toLatin1() == 0  || ch.isSpace())
                return false;
        return true;
    }
    static inline auto isValid(QVariant const& value) noexcept {
        return isValid(value.toString());
    }

public:
    explicit LoginValidator(QObject* parent = nullptr) noexcept
        : inherited(parent) {
    }
    virtual State validate(QString& str, int& pos) const override final {
        if(pos < 0 || pos > str.length() || (!str.isEmpty() && str[0].isDigit()))
            return QValidator::Invalid;

        return isValid(str)
              ? QValidator::Acceptable
              : QValidator::Invalid;
    }
};
}  // end namespace gt

