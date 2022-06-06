#pragma once
#include <QList>
#include <QString>
#include <QLocale>
#include "include/convert/integer.h"
#include "include/typedefs.h"

using gid_t = uint;
using gid_list_t = QList<gid_t>;

template<class T>
inline QString gidToString(T const& gid) noexcept {
    return IPS_STR("gid%1").arg(gid);
}
inline QString gidToString(QVariant const& gid) noexcept {
    return gidToString<QString>(gid.toString());
}
inline gid_t toGid(QString const& str, gid_t def = 0U) noexcept {
    if(convert::isUint(str))
        return convert::toUint(str);

    return str.size() <= 3
        ? def
        : convert::toUint(str.right(str.size() - 3), def);
}
inline gid_t toGid(QVariant const& str, gid_t def = 0U) noexcept {
    return toGid(str.toString(), def);
}
inline bool isGid(QString const str) noexcept {
   bool is = false;
   QLocale().toUInt(str, &is);
   return is;
}
inline bool isGid(QVariant const var) noexcept {
    return isGid(var.toString());
}
