#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class StationType: std::int8_t {
      Invalid = 0
    , Unknown = 1
    , UM120   = 2
    , UM240   = 3
    , PDO32   = 4
    , PDO16   = 5
    , PDO8    = 6
    , UPW2    = 7
    , UPW4    = 8
    , UPW6    = 9
    , SL1     = 10
};
inline QString toString(StationType type) noexcept {
    if (type == StationType::Invalid)     return IPS_STR("invalid");
    if (type == StationType::Unknown)     return IPS_STR("unknown");
    if (type == StationType::UM120)       return IPS_STR("UM120");
    if (type == StationType::UM240)       return IPS_STR("UM240");
    if (type == StationType::PDO32)       return IPS_STR("PDO32");
    if (type == StationType::PDO16)       return IPS_STR("PDO16");
    if (type == StationType::PDO8)        return IPS_STR("PDO8");
    if (type == StationType::UPW2)        return IPS_STR("UPW2");
    if (type == StationType::UPW4)        return IPS_STR("UPW4");
    if (type == StationType::UPW6)        return IPS_STR("UPW6");
    if (type == StationType::SL1)         return IPS_STR("SL1");

    LOG_ERROR(0, "Unknown StationType of (%1)", convert::to_utype(type));
    return IPS_STR("Error");
}
inline bool fromString(QString const& str, StationType& val , QLocale const& = {}) noexcept {
    if (str == IPS_STR("invalid"))      val = StationType::Invalid;
    else if (str == IPS_STR("unknown")) val = StationType::Unknown;
    else if (str == IPS_STR("UM120"))   val = StationType::UM120;
    else if (str == IPS_STR("UM240"))   val = StationType::UM240;
    else if (str == IPS_STR("PDO32"))   val = StationType::PDO32;
    else if (str == IPS_STR("PDO16"))   val = StationType::PDO16;
    else if (str == IPS_STR("PDO8"))    val = StationType::PDO8;
    else if (str == IPS_STR("UPW2"))    val = StationType::UPW2;
    else if (str == IPS_STR("UPW4"))    val = StationType::UPW4;
    else if (str == IPS_STR("UPW6"))    val = StationType::UPW6;
    else if (str == IPS_STR("SL1"))     val = StationType::SL1;
    else {
         LOG_ERROR(0, "Invalid station::StationType value (%1)", str);
         val = StationType::Invalid;
         return false;
    }
    return true;
}

/* begin namespace station_type */
namespace station_type {
using items_t = std::array<StationType,9>;

static inline items_t defaultStationTypes() noexcept {
    return {            
             StationType::UM120
           , StationType::UM240
           , StationType::PDO32
           , StationType::PDO16
           , StationType::PDO8
           , StationType::UPW2
           , StationType::UPW4
           , StationType::UPW6
           , StationType::SL1
    };
}
static inline void addItem(QComboBox* box, StationType type) noexcept {
    box->addItem(toString(type), convert::to_utype(type));
}
    inline void fill(QComboBox* box) noexcept {
        for(auto c: defaultStationTypes())
            addItem(box, c);
    }
} // end namespace station_type

Q_DECLARE_METATYPE(StationType)
