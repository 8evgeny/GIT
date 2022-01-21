#pragma once
#include <QList>
#include <QString>
#include <QLocale>
#include "include/convert/integer.h"
#include "include/typedefs.h"

using cid_t = uint;
using cid_list_t = QList<cid_t>;

template<class T>
inline QString cidToString(T const& cid) noexcept {
    return IPS_STR("CID%1").arg(cid);
}
inline QString cidToString(QVariant const& cid) noexcept {
    return cidToString<QString>(cid.toString());
}
inline cid_t toCid(QString const& str, cid_t def = 0U) noexcept {
    if(convert::isUint(str))
        return convert::toUint(str);

    return str.size() <= 3
        ? def
        : convert::toUint(str.right(str.size() - 3), def);
}
inline cid_t toCid(QVariant const& str, cid_t def = 0U) noexcept {
    return toCid(str.toString(), def);
}
inline bool isCid(QString const str) noexcept {
   bool is = false;
   QLocale().toUInt(str, &is);
   return is;
}
inline bool isCid(QVariant const var) noexcept {
    return isCid(var.toString());
}
