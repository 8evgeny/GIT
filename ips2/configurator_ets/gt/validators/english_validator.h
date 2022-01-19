/**
 * \file      ojects/cnf/include/validators/english_validator.h 
 * \brief     The English_validator class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 06(th), 2020, 12:13 MSK
 * \updated   November  (the) 06(th), 2020, 12:13 MSK
 * \TODO      
**/
#pragma once
#include <QValidator>

/** \namespace gt */
namespace gt {

/**
 * \code
    ui->m_name->setValidator(new gt::EnglishValidator(ui->m_name));
 * \endcode
 **/
class EnglishValidator final: public QValidator {
public:
    using class_name = EnglishValidator;
    using inherited  = QValidator;

public:
    static inline auto isValid(QString const& value) noexcept {
        for(auto const ch: value)
            if(ch.toLatin1() == 0)
                return false;
        return true;
    }
    static inline auto isValid(QVariant const& value) noexcept {
        return isValid(value.toString());
    }

public:
    explicit EnglishValidator(QObject* parent = nullptr) noexcept
        : inherited(parent) {
    }
    virtual State validate(QString& str, int& pos) const override final {
        if(pos < 0 || pos > str.length())
            return QValidator::Invalid;
        if(pos < str.length())
            return isValid(str.mid(0, pos))
                      ? QValidator::Acceptable
                      : QValidator::Intermediate;
        return isValid(str)
              ? QValidator::Acceptable
              : QValidator::Invalid;
    }
};
}  // end namespace gt

