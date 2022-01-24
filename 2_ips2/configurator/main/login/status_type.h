#pragma once

#include <array>
#include <QComboBox>
#include <gt/out.h>
#include <gt/convert/enum.h>
#include "include/tr.h"
#include "include/log.h"

enum class StatusType : std::int8_t {
     Invalid   = 0
    , Unknown  = 1
    , Enabled  = 2
    , Disabled = 3
};

/** begin namespace status_type */
namespace status_type {
inline QString toTr(StatusType type) noexcept {
    if (type == StatusType::Invalid)
        return iType::tr("Invalid");
    if (type == StatusType::Unknown)
        return iType::tr("Unknown");
    if (type == StatusType::Enabled)
        return iType::tr("Enabled");
    if (type == StatusType::Disabled)
        return iType::tr("Disabled");

    LOG_ERROR(0, "Unknown type of StatusType (%1)", gt::convert::to_utype(type));
    return QStringLiteral("Invalid");
}
inline QString toString(StatusType type) noexcept {
    if (type == StatusType::Invalid)
        return QStringLiteral("Invalid");
    if (type == StatusType::Invalid)
        return QStringLiteral("Unknown");
    if (type == StatusType::Enabled)
        return QStringLiteral("Enabled");
    if (type == StatusType::Disabled)
        return QStringLiteral("Disabled");

    LOG_ERROR(0, "Unknown type of StatusType (%1)", gt::convert::to_utype(type));
    return QStringLiteral("Error");
}
inline bool fromString(QString const& str, StatusType& val
                                  , QLocale const& = {}) noexcept {
    if (str == QStringLiteral("Unknown"))
        val = StatusType::Unknown;
    else if (str == QStringLiteral("Enabled"))
        val = StatusType::Enabled;
    else if (str == QStringLiteral("Disabled"))
        val = StatusType::Disabled;
    else {
        LOG_ERROR(0, "Invalid StatusType value (%1)", str);
        val = StatusType::Invalid;
        return false;
    }
    return true;
}
static inline std::array<StatusType, 2> defaultTypes() noexcept {
    return {
            StatusType::Enabled
          , StatusType::Disabled
    };
}
static inline void addItem(QComboBox* box, StatusType type) noexcept {
    box->addItem(toTr(type), gt::convert::to_utype(type));
}
inline void fill(QComboBox* box) noexcept {
    for(auto c: defaultTypes())
        addItem(box, c);
}

} // end namespace status_type

inline QDebug operator<<(QDebug dbg, StatusType type) noexcept {
    return dbg.nospace() << status_type::toString(type);
}
inline std::ostream& operator<<(std::ostream& out, StatusType type) noexcept {
    return gt::cout(out, type);
}
