/**
 * \file      ojects/cnf/include/string_algo.h 
 * \brief     The String_algo class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.0
 * \created   November  (the) 19(th), 2020, 17:26 MSK
 * \updated   November  (the) 19(th), 2020, 17:26 MSK
 * \TODO      
**/
#pragma once
#include <QLineEdit>
#include <QTextEdit>
#include "convert/string.h"

/** \namespace gt */
namespace gt {
    static inline QString trim(QString const& e) noexcept {
        return e.trimmed();
    }
    static inline QString trim(QLineEdit* e) noexcept {
        return e->text().trimmed();
    }
    static inline QString trim(QTextEdit* e) noexcept {
       return gt::convert::toPlainText(e->toPlainText()).trimmed();
    }

}  // end namespace gt
