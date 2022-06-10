/**
 * \file      ojects/cnf/include/checker.h 
 * \brief     The Checker class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.2
 * \created   November  (the) 19(th), 2020, 16:35 MSK
 * \updated   November  (the) 19(th), 2020, 20:38 MSK
 * \TODO      
**/
#pragma once
#include <QLineEdit>
#include <QTextEdit>
#include <QHostAddress>
#include "string_algo.h"
#include "convert/string.h"
#include "validators/login_validator.h"
#include "validators/english_validator.h"
#include "validators/pass_validator.h"
#include "validators/email_validator.h"
#include "validators/num_validator.h"
#include "validators/port_validator.h"
#include "validators/ip_validator.h"

/** \namespace gt::checker */
namespace gt::checker {
    static inline bool textSize(QString const& text, int min, int max) noexcept {
        auto const s = gt::trim(text).size();
        return min <= s && s <= max;
    }
    template <class T>
    static inline bool textSize(T* e, int min, int max) noexcept {
        auto const s = gt::trim(e).size();
        return min <= s && s <= max;
    }
    template <class T>
    static inline bool Int(T* e, int min, int max) noexcept {
        auto const& s = gt::trim(e);
        auto const val = gt::convert::toInt(s);
        return min <= val && val < max;
    }
    template <class T>
    static inline bool Uint(T* e, uint min, uint max) noexcept {
        auto const& s = gt::trim(e);
        auto const val = gt::convert::toUint(s);
        return min <= val && val < max;
    }
    template <class T>
    static inline bool Double(T* e, int min, int max) noexcept {
        auto const& s = gt::trim(e);
        auto const val = gt::convert::toDouble(s);
        return min <= val && val < max;
    }
    template <class T>
    static inline bool login(T* e) noexcept {
        return textSize(e, 3, 32)
            && gt::LoginValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool loginEmpty(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return textSize(e, 0, 32)
            && gt::LoginValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool num(T* e) noexcept {
        return textSize(e, 1, 5)
                && gt::NumValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool name(T* e) noexcept {
        return textSize(e, 3, 64);
    }
    template <class T>
    static inline bool emptyName(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return textSize(e, 0, 64);
    }
    template <class T>
    static inline bool english(T* e, int min, int max) noexcept {
        return textSize(e, min, max)
                && gt::EnglishValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool englishEmpty(T* e, int max) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return textSize(e, 0, max)
                && gt::EnglishValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool pass(T* e) noexcept {
        return textSize(e, 3, 64) && 
            gt::PassValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool passEmpty(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return textSize(e, 0, 64) && 
            gt::PassValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool desc(T* e) noexcept {
        return textSize(e, 0, 256);
    }
    template <class T>
    static inline bool email(T* e) noexcept {
        return gt::EmailValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool emailEmpty(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return gt::EmailValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool ip(T* e) noexcept {
        return gt::IpValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool ipEmpty(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return gt::IpValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool port(T* e) noexcept {
        return gt::PortValidator::isValid(gt::trim(e));
    }
    template <class T>
    static inline bool portEmpty(T* e) noexcept {
        if(gt::trim(e).isEmpty())
            return true;

        return gt::PortValidator::isValid(gt::trim(e));
    }

}  // end namespace gt::checker
