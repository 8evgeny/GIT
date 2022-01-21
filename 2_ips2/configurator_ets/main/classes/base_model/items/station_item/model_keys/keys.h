#pragma once
#include <QList>
#include <QVariant>
#include <QString>
#include <QLocale>
#include "include/convert/integer.h"

using key_type = uint;
using key_list_t = QList<key_type>;

inline bool isKey(QString const str) noexcept {
   bool is = false;
   QLocale().toUInt(str, &is);
   return is;
}
inline bool isKey(QVariant const var) noexcept {
    return isKey(var.toString());
}
