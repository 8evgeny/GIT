#pragma once
#include <QString>
#include <QList>
#include "include/convert/integer.h"
#include "include/typedefs.h"

using gr_t = uint;
using gr_list_t = QList<gr_t>;

template<class T>
inline QString gtToString(T const& gt) noexcept {
    if (gt == IPS_STR("None"))
        return IPS_STR("None");
    return IPS_STR("GID%1").arg(gt);
}
inline QString gtToString(QVariant const& gt) noexcept {
    return gtToString<QString>(gt.toString());
}
inline gr_t gtFromString(QString const& str, gr_t def = 0U) noexcept {
    if(convert::isUint(str))
        return convert::toUint(str);

    return str.size() <= 2
        ? def
        : convert::toUint(str.right(str.size() - 2), def);
}
inline gr_t gtFromString(QVariant const& str, gr_t def = 0u) noexcept {
    return gtFromString(str.toString(), def);
}
