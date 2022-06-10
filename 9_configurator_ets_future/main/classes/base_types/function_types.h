#pragma once
#include "include/log.h"
#include "include/convert/enum.h"
#include <QtCore>

/*enum class FunctionType: std::int32_t {
    Invalid                  = 0	//
    , Unknown                = 1	//
    , None                   = 2	//Не назначено (эт1)
    , Simplex                = 3	//Симплексная связь (эт1)
    , Duplex                 = 4	//Дуплексная связь (эт1)
    , GroupPA                = 5	//Групповая связь (эт?)
    , ConferenceStart        = 6	//Вступить в конференцию (эт?)
    , ConferenceBreak        = 7	//Покинуть конференцию (эт?)
    , ControlLineEnable      = 8	//Замкнуть контакт, для платы SL (эт?)
    , ControlLineDisable     = 9	//Разомкнуть контакт, для платы SL (эт?)
    , ControlLineToggle      = 10	//Переключить контакт, для платы SL (эт?)
    , DynamicCcallGroupStart = 11	//Вход в циркулярную связь (эт?)
    , DynamiCallGroupBreak   = 12	//Выход из циркулярной связи (эт?)
    , HangUp                 = 13	//Выход из конференции (эт?)
    , PlayAlarm              = 14	//Включить воспроизведение аудио записи (эт?)
    , StopAlarm              = 15	//Выключить воспроизведение аудио записи (эт?)
    , MicMute                = 16	//Отключить встроенный микрофон (эт?)
    , SpeakerMute            = 17	//Отключить встроенный громкоговоритель (эт?)
    , Cancel                 = 18	//Отбой (эт?)
	, DuplexAuto             = 19	//Дуплексная связь с автоответом (эт1)
};
*/
enum class FunctionType: std::int32_t {
    Invalid						= -1	//
    , Unknown					= 0	//
    , None						= 1	//Нет
    , SimplexPA					= 2	//Симплекс
    , Duplex					= 3	//Дуплекс
	, DuplexAuto				= 4	//Дуплекс авто
    , GroupPA					= 5	//Групповая связь
	, Circular					= 6	//Циркуляр
	, Conference				= 7	//Конференция
	, AlarmOnOff				= 8	//Оповещение Вкл/Выкл
	, MicOnOff					= 9	//МК Вкл/Выкл
    , SpeakerOnOff				= 10//ГР Вкл/Выкл
	, LineIndicator				= 11//Индикатор линии
	, ContactOnOff				= 12//Контакт Вкл/Выкл
	, ContactOnOffSimplexPA		= 13//Контакт Вкл/Выкл Симплекс
	, ContactOnOffDuplexAuto	= 14//Контакт Вкл/Выкл Дуплекс Авто
	, LineOnOffSimplexPA		= 15//Трансляция Вкл/Выкл Симлекс
	, LineOnOffDuplexAuto		= 16//Трансляция Вкл/Выкл Дуплекс Авто
};

/*
inline QString toString(FunctionType FunctionType = FunctionType::Unknown) noexcept {
    if (FunctionType == FunctionType::Invalid)                return IPS_STR("Invalid");
    if (FunctionType == FunctionType::Unknown)                return IPS_STR("Unknown");
    if (FunctionType == FunctionType::None)                   return IPS_STR("None");
    if (FunctionType == FunctionType::Simplex)                return IPS_STR("Simplex");
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
	if (FunctionType == FunctionType::DuplexAuto)             return IPS_STR("Duplex auto");

    LOG_ERROR(0, "Unknown FunctionType of (%1)", convert::to_utype(FunctionType));
    return IPS_STR("Error");
}
*/
inline QString toString(FunctionType FunctionType = FunctionType::Unknown) noexcept {
    if (FunctionType == FunctionType::Invalid)					return QObject::tr("Invalid");
    if (FunctionType == FunctionType::Unknown)					return QObject::tr("Unknown");
    if (FunctionType == FunctionType::None)						return QObject::tr("None");
    if (FunctionType == FunctionType::SimplexPA)                return QObject::tr("Simplex PA");
    if (FunctionType == FunctionType::Duplex)					return QObject::tr("Duplex");
	if (FunctionType == FunctionType::DuplexAuto)               return QObject::tr("Duplex Auto");
    if (FunctionType == FunctionType::GroupPA)					return QObject::tr("Group PA");
    if (FunctionType == FunctionType::Circular)					return QObject::tr("Circular");
    if (FunctionType == FunctionType::Conference)				return QObject::tr("Conference");
    if (FunctionType == FunctionType::AlarmOnOff)				return QObject::tr("Alarm on/off");
    if (FunctionType == FunctionType::MicOnOff)					return QObject::tr("Mic on/off");
    if (FunctionType == FunctionType::SpeakerOnOff)				return QObject::tr("Speaker on/off");
    if (FunctionType == FunctionType::LineIndicator)			return QObject::tr("Line indicator");
    if (FunctionType == FunctionType::ContactOnOff)				return QObject::tr("Contact on/off");
    if (FunctionType == FunctionType::ContactOnOffSimplexPA)    return QObject::tr("Contact on/off Simplex PA");
    if (FunctionType == FunctionType::ContactOnOffDuplexAuto)	return QObject::tr("Contact on/off Duplex Auto");
    if (FunctionType == FunctionType::LineOnOffSimplexPA)       return QObject::tr("Line on/off Simplex PA");
    if (FunctionType == FunctionType::LineOnOffDuplexAuto)      return QObject::tr("Line on/off Duplex Auto");
    LOG_ERROR(0, "Unknown FunctionType of (%1)", convert::to_utype(FunctionType));
    return IPS_STR("Error");
}
/* begin namespace function_type */
/*
namespace function_type {
inline bool fromString(QString const& str, FunctionType& val, QLocale const& = {}) noexcept {
    if (str == iFunction::tr("Invalid"))                       val = FunctionType::Invalid;
    else if (str == iFunction::tr("Unknown"))                  val = FunctionType::Unknown;
    else if (str == iFunction::tr("None"))                     val = FunctionType::None;
    else if (str == iFunction::tr("Simplex"))                  val = FunctionType::Simplex;
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
	else if (str == iFunction::tr("Duplex auto"))              val = FunctionType::DuplexAuto;
    else {
         LOG_ERROR(0, "Invalid station::FunctionType value (%1)", str);
         return false;
    }
    return true;
}
*/
namespace function_type {
inline bool fromString(QString const& str, FunctionType& val, QLocale const& = {}) noexcept {
    if (str == QObject::tr("Invalid"))							val = FunctionType::Invalid;
    else if (str == QObject::tr("Unknown"))						val = FunctionType::Unknown;
    else if (str == QObject::tr("None"))							val = FunctionType::None;
    else if (str == QObject::tr("Simplex PA"))					val = FunctionType::SimplexPA;
    else if (str == QObject::tr("Duplex"))						val = FunctionType::Duplex;
    else if (str == QObject::tr("Duplex Auto"))					val = FunctionType::DuplexAuto;
    else if (str == QObject::tr("Group PA"))						val = FunctionType::GroupPA;
    else if (str == QObject::tr("Circular"))						val = FunctionType::Circular;
    else if (str == QObject::tr("Conference"))					val = FunctionType::Conference;
    else if (str == QObject::tr("Alarm on/off"))					val = FunctionType::AlarmOnOff;
    else if (str == QObject::tr("Mic on/off"))					val = FunctionType::MicOnOff;
    else if (str == QObject::tr("Speaker on/off"))				val = FunctionType::SpeakerOnOff;
    else if (str == QObject::tr("Line indicator"))				val = FunctionType::LineIndicator;
    else if (str == QObject::tr("Contact on/off"))                val = FunctionType::ContactOnOff;
    else if (str == QObject::tr("Contact on/off Simplex PA"))     val = FunctionType::ContactOnOffSimplexPA;
    else if (str == QObject::tr("Contact on/off Duplex Auto"))	val = FunctionType::ContactOnOffDuplexAuto;
    else if (str == QObject::tr("Line on/off Simplex PA"))        val = FunctionType::LineOnOffSimplexPA;
    else if (str == QObject::tr("Line on/off Duplex Auto"))       val = FunctionType::LineOnOffDuplexAuto;
    else {
         LOG_ERROR(0, "Invalid station::FunctionType value (%1)", str);
         return false;
    }
    return true;
}
/*
using items_t = std::array<FunctionType,18>;
inline items_t defaultFunctionTypes() noexcept {
    return {
            FunctionType::None
            ,FunctionType::Simplex
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
			,FunctionType::DuplexAuto
    };
}
*/
using items_t = std::array<FunctionType,16>;
inline items_t defaultFunctionTypes() noexcept {
    return {
            FunctionType::None
            ,FunctionType::SimplexPA
            ,FunctionType::Duplex
            ,FunctionType::DuplexAuto
            ,FunctionType::GroupPA
            ,FunctionType::Circular
            ,FunctionType::Conference
            ,FunctionType::AlarmOnOff
            ,FunctionType::MicOnOff
            ,FunctionType::SpeakerOnOff
            ,FunctionType::LineIndicator
            ,FunctionType::ContactOnOff
            ,FunctionType::ContactOnOffSimplexPA
            ,FunctionType::ContactOnOffDuplexAuto
            ,FunctionType::LineOnOffSimplexPA
            ,FunctionType::LineOnOffDuplexAuto
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
