#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class FunctionType: std::int32_t {
    Invalid                  = 0
    , Unknown                = 1
    , None                   = 2
    , PA                     = 3
    , Duplex                 = 4
    , GroupPA                = 5
    , ConferenceStart        = 6
    , ConferenceBreak        = 7
    , ControlLineEnable      = 8
    , ControlLineDisable     = 9
    , ControlLineToggle      = 10
    , DynamicCcallGroupStart = 11
    , DynamiCallGroupBreak   = 12
    , HangUp                 = 13
    , PlayAlarm              = 14
    , StopAlarm              = 15
    , MicMute                = 16
    , SpeakerMute            = 17
    , Cancel                 = 18
};
inline QString toString(FunctionType FunctionType = FunctionType::Unknown) noexcept {
    if (FunctionType == FunctionType::Invalid)                return IPS_STR("Invalid");
    if (FunctionType == FunctionType::Unknown)                return IPS_STR("Unknown");
    if (FunctionType == FunctionType::None)                   return IPS_STR("None");
    if (FunctionType == FunctionType::PA)                     return IPS_STR("PA");
    if (FunctionType == FunctionType::Duplex)                 return IPS_STR("Duplex");
    if (FunctionType == FunctionType::GroupPA)                return IPS_STR("Group PA");
    if (FunctionType == FunctionType::ConferenceStart)        return IPS_STR("Conference start");
    if (FunctionType == FunctionType::ConferenceBreak)        return IPS_STR("Conference break");
    if (FunctionType == FunctionType::ControlLineEnable)      return IPS_STR("Control line enable");
    if (FunctionType == FunctionType::ControlLineDisable)     return IPS_STR("Control line disable");
    if (FunctionType == FunctionType::ControlLineToggle)      return IPS_STR("Control line toggle");
    if (FunctionType == FunctionType::DynamicCcallGroupStart) return IPS_STR("Dynamic call group start");
    if (FunctionType == FunctionType::DynamiCallGroupBreak)   return IPS_STR("Dynamic call group break");
    if (FunctionType == FunctionType::HangUp)                 return IPS_STR("Hang up");
    if (FunctionType == FunctionType::PlayAlarm)              return IPS_STR("Play alarm");
    if (FunctionType == FunctionType::StopAlarm)              return IPS_STR("Stop alarm");
    if (FunctionType == FunctionType::MicMute)                return IPS_STR("Mic mute");
    if (FunctionType == FunctionType::SpeakerMute)            return IPS_STR("Speaker mute");
    if (FunctionType == FunctionType::Cancel)                 return IPS_STR("Cancel");

    LOG_ERROR(0, "Unknown FunctionType of (%1)", convert::to_utype(FunctionType));
    return IPS_STR("Error");
}
/* begin namespace function_type */
namespace function_type {
inline bool fromString(QString const& str, FunctionType& val, QLocale const& = {}) noexcept {
    if (str == iFunction::tr("Invalid"))                       val = FunctionType::Invalid;
    else if (str == iFunction::tr("Unknown"))                  val = FunctionType::Unknown;
    else if (str == iFunction::tr("None"))                     val = FunctionType::None;
    else if (str == iFunction::tr("PA"))                       val = FunctionType::PA;
    else if (str == iFunction::tr("Duplex"))                   val = FunctionType::Duplex;
    else if (str == iFunction::tr("Group PA"))                 val = FunctionType::GroupPA;
    else if (str == iFunction::tr("Conference start"))         val = FunctionType::ConferenceStart;
    else if (str == iFunction::tr("Conference break"))         val = FunctionType::ConferenceBreak;
    else if (str == iFunction::tr("Control line enable"))      val = FunctionType::ControlLineEnable;
    else if (str == iFunction::tr("Control line disable"))     val = FunctionType::ControlLineDisable;
    else if (str == iFunction::tr("Control line toggle"))      val = FunctionType::ControlLineToggle;
    else if (str == iFunction::tr("Dynamic call group start")) val = FunctionType::DynamicCcallGroupStart;
    else if (str == iFunction::tr("Dynamic call group break")) val = FunctionType::DynamiCallGroupBreak;
    else if (str == iFunction::tr("Hang up"))                  val = FunctionType::HangUp;
    else if (str == iFunction::tr("Play alarm"))               val = FunctionType::PlayAlarm;
    else if (str == iFunction::tr("Stop alarm"))               val = FunctionType::StopAlarm;
    else if (str == iFunction::tr("Mic mute"))                 val = FunctionType::MicMute;
    else if (str == iFunction::tr("Speaker mute"))             val = FunctionType::SpeakerMute;
    else if (str == iFunction::tr("Cancel"))                   val = FunctionType::Cancel;
    else {
         LOG_ERROR(0, "Invalid station::FunctionType value (%1)", str);
         return false;
    }
    return true;
}
using items_t = std::array<FunctionType,17>;
inline items_t defaultFunctionTypes() noexcept {
    return {
            FunctionType::None
            ,FunctionType::PA
            ,FunctionType::Duplex
            ,FunctionType::GroupPA
            ,FunctionType::ConferenceStart
            ,FunctionType::ConferenceBreak
            ,FunctionType::ControlLineEnable
            ,FunctionType::ControlLineDisable
            ,FunctionType::ControlLineToggle
            ,FunctionType::DynamicCcallGroupStart
            ,FunctionType::DynamiCallGroupBreak
            ,FunctionType::HangUp
            ,FunctionType::PlayAlarm
            ,FunctionType::StopAlarm
            ,FunctionType::MicMute
            ,FunctionType::SpeakerMute
            ,FunctionType::Cancel
    };
}
static inline void addItem(QComboBox* box, FunctionType t) noexcept {
    box->addItem(toString(t), convert::to_utype(t));
}
inline void fill(QComboBox* box) noexcept {    
    for(auto c: defaultFunctionTypes())
        addItem(box, c);
}
inline QDebug operator<<(QDebug dbg, FunctionType type) noexcept {
    return dbg.nospace() << toString(type);
}
} // end namespace function_type

Q_DECLARE_METATYPE(FunctionType)
