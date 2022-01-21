/**
 * \file      c:/projects/niokr/conan/conan-libgt/sources/gt/validators/email_validator.h 
 * \brief     The Email_validator class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 21:40 MSK
 * \updated   November  (the) 19(th), 2020, 21:40 MSK
 * \TODO      
**/
#pragma once
#include <QRegExp>

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    #include <QRegularExpressionValidator>
    #define RegValidator  QRegularExpressionValidator
#else
    #include <QRegExpValidator>
    #define RegValidator  QRegExpValidator
#endif

/** \namespace gt */
namespace gt {

class EmailValidator: public RegValidator {
public:
    using class_name = EmailValidator;
    using inherited  = RegValidator;

public:
    static inline QString email() noexcept {
        return QStringLiteral("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    }
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    static inline auto emailReg() noexcept {
        QRegularExpression reg(email());
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }
#else
    static inline auto emailReg() noexcept {
        QRegExp reg(email());
        reg.setCaseSensitivity(Qt::CaseInsensitive);
        reg.setPatternSyntax(QRegExp::RegExp);
        return reg;
    }
#endif
    static inline auto isValid(QString const& value) noexcept {
        auto const& reg = emailReg();
        return reg.exactMatch(value);
    }
    static inline auto isValid(QVariant const& value) noexcept {
        return isValid(value.toString());
    }

public:
    explicit EmailValidator(QObject* parent = nullptr) noexcept
        : inherited(emailReg(), parent) {
    }
}; 
} // end namespace gt
