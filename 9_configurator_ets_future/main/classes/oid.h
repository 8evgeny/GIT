#pragma once
#include <QList>
#include <QString>
#include <QLocale>
#include "include/convert/integer.h"
#include "include/typedefs.h"

using oid_t = uint;
using oid_list_t = QList<oid_t>;

template<class T>
inline QString oidToString(T const& oid) noexcept {
    return IPS_STR("oid%1").arg(oid);
}
inline QString oidToString(QVariant const& oid) noexcept {
    return oidToString<QString>(oid.toString());
}
inline oid_t toOid(QString const& str, oid_t def = 0U) noexcept {
    if(convert::isUint(str))
        return convert::toUint(str);

    return str.size() <= 3
        ? def
        : convert::toUint(str.right(str.size() - 3), def);
}
inline oid_t toOid(QVariant const& str, oid_t def = 0U) noexcept {
    return toOid(str.toString(), def);
}
inline bool isOid(QString const str) noexcept {
   bool is = false;
   QLocale().toUInt(str, &is);
   return is;
}
inline bool isOid(QVariant const var) noexcept {
    return isOid(var.toString());
}
