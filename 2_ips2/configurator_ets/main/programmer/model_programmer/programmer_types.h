#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class StatusIPType: std::int8_t {
    Invalid         = 0
    , Unknown       = 1
    , Available     = 2
    , Inaccessible  = 3
};
inline QString toString(StatusIPType StatusIPType) noexcept {
    if (StatusIPType == StatusIPType::Invalid)                return IPS_STR("Invalid");
    if (StatusIPType == StatusIPType::Unknown)                return IPS_STR("Unknown");
    if (StatusIPType == StatusIPType::Available)              return IPS_STR("Available");
    if (StatusIPType == StatusIPType::Inaccessible)           return IPS_STR("Inaccessible");
    LOG_ERROR(0, "Unknown FunctionType of (%1)", convert::to_utype(StatusIPType));
    return IPS_STR("Error");
}
inline bool fromString(QString const& str, FunctionType& val, QLocale const& = {}) noexcept {
    if (str == iFunction::tr("Invalid"))                       val = FunctionType::Invalid;
    else if (str == iFunction::tr("Unknown"))                  val = FunctionType::Unknown;
    else if (str == iFunction::tr("None"))                     val = FunctionType::None;
    else if (str == iFunction::tr("PA"))                       val = FunctionType::PA;
    else {
         LOG_ERROR(0, "Invalid station::FunctionType value (%1)", str);
         return false;
    }
    return true;
}
