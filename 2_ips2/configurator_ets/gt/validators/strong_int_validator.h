/**
 * \file      conan/conan-libgt/sources/gt/validators/int_validator.h
 * \brief     The Email_validator class provides
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 21:40 MSK
 * \updated   November  (the) 19(th), 2020, 21:40 MSK
 * \TODO
**/
#pragma once
#include <QValidator>
#include "../convert/integer.h"

/** \namespace gt */
namespace gt {

/**
 * \code
    ui->m_login->setValidator(new gt::StrongIntValidator(ui->m_login));
 * \endcode
 **/
class StrongIntValidator: public QValidator {
 public:
    using class_name = StrongIntValidator;
    using inherited  = QValidator;

 public:
    static inline auto isValid(QString const& value, int min, int max) noexcept {
        auto const val = convert::toInt(value);
        return min <= val && val < max;
    }
    static inline auto isValid(QVariant const& value, int min, int max) noexcept {
        return isValid(value.toString(), min, max);
    }

 public:
    explicit StrongIntValidator(QObject* parent = nullptr) noexcept
        : inherited(parent) {
    }
    explicit StrongIntValidator(int min, int max, QObject* parent = nullptr) noexcept
        : inherited(parent)
        , m_min(min)
        , m_max(max){
    }
    void setMinMax(int min, int max) noexcept {
        setMinimum(min);
        setMaximum(max);
    }
    void setMaximum(int max) noexcept {
        m_max = max;
    }
    void setMinimum(int min) noexcept {
        m_min = min;
    }
    int maximum() const noexcept {
        return m_max;
    }
    int minimum() const noexcept {
        return m_min;
    }
    virtual State validate(QString& str, int&) const override final {
        if(str.isEmpty())
            return QValidator::Intermediate;

        return isValid(str, m_min, m_max)
              ? QValidator::Acceptable
              : QValidator::Invalid;
    }

 private:
    int m_min = 0;
    int m_max = std::numeric_limits<int>::max();

};
}  // end namespace gt
