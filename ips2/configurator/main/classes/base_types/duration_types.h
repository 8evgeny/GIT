#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class DurationType: std::int32_t {
    Invalid                  = 0
    , Unknown                = 1
    , ONCE                   = 2
    , INFINITE               = 3
    , times         = 4
    , seconds       = 5
};
/* begin namespace duration_type */
namespace duration_type {
inline QString toString(DurationType DurationType = DurationType::Unknown) noexcept {
    if (DurationType == DurationType::Invalid)                return IPS_STR("Invalid");
    if (DurationType == DurationType::Unknown)                return IPS_STR("Unknown");
    if (DurationType == DurationType::ONCE)                   return IPS_STR("ONCE");
    if (DurationType == DurationType::INFINITE)               return IPS_STR("INFINITE");
    if (DurationType == DurationType::times)                  return IPS_STR("times");
    if (DurationType == DurationType::seconds)                return IPS_STR("seconds");
    LOG_ERROR(0, "Unknown DurationType of (%1)", convert::to_utype(DurationType));
    return IPS_STR("Error");
}
inline DurationType fromString(QString const& str, QLocale const& = {}) noexcept {
    if (str == iFunction::tr("Invalid"))                       return DurationType::Invalid;
    else if (str == iFunction::tr("Unknown"))                  return DurationType::Unknown;
    else if (str == iFunction::tr("ONCE"))                     return DurationType::ONCE;
    else if (str == iFunction::tr("INFINITE"))                 return DurationType::INFINITE;
    else if (str == iFunction::tr("times"))                    return DurationType::times;
    else if (str == iFunction::tr("seconds"))                  return DurationType::seconds;
    return {};
}
using items_t = std::array<DurationType, 6>;
inline items_t defaultDurationTypes() noexcept {
    return {
            DurationType::ONCE
            ,DurationType::INFINITE
            ,DurationType::times
            ,DurationType::seconds
    };
}
static inline void addItem(QComboBox* box, DurationType t) noexcept {
    box->addItem(toString(t), convert::to_utype(t));
}
inline void fill(QComboBox* box) noexcept {
    for(auto c: defaultDurationTypes())
        addItem(box, c);
}
inline QDebug operator<<(QDebug dbg, DurationType type) noexcept {
    return dbg.nospace() << toString(type);
}
} // end namespace duration_type

Q_DECLARE_METATYPE(DurationType)
