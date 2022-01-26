#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class SipType: std::int8_t {
      Invalid = 0
    , Unknown = 1
    , SIP     = 2
    , ModBus  = 3
    , SNMP    = 4
};
inline QString toString(SipType type) noexcept {
    if (type == SipType::Invalid)     return IPS_STR("invalid");
    if (type == SipType::Unknown)     return IPS_STR("unknown");
    if (type == SipType::SIP)         return IPS_STR("SIP");
    if (type == SipType::ModBus)      return IPS_STR("ModBus");
    if (type == SipType::SNMP)        return IPS_STR("SNMP");
    LOG_ERROR(0, "Unknown SipType of (%1)", convert::to_utype(type));
    return IPS_STR("Error");
}

inline bool fromString(QString const& str, SipType& val , QLocale const& = {}) noexcept {
    if (str == IPS_STR("invalid"))      val = SipType::Invalid;
    else if (str == IPS_STR("unknown")) val = SipType::Unknown;
    else if (str == IPS_STR("SIP"))     val = SipType::SIP;
    else if (str == IPS_STR("ModBus"))  val = SipType::ModBus;
    else if (str == IPS_STR("SNMP"))    val = SipType::SNMP;
    else {
         LOG_ERROR(0, "Invalid station::SipType value (%1)", str);
         val = SipType::Invalid;
         return false;
    }
    return true;
}
