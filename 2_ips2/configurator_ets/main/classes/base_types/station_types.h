#pragma once
#include "include/log.h"
#include "include/convert/enum.h"
#include <QtCore>

enum class StationType: std::int8_t {
      Invalid	 = -1
    , Unknown	 = 0
    , PDO16		 = 1
    , PDO16N	 = 2
    , PDO32		 = 3
    , PDO32N	 = 4
	, PDO48		 = 5
    , UPV2		 = 10
    , UPV4		 = 11
    , UPV6		 = 12
    , UPV2N		 = 13
    , UPV4N		 = 14
    , UPV_A		 = 15
    , SL1		 = 20
    , SL2		 = 21
    , UM_300_2   = 30
};

inline QString toString(StationType type) noexcept {
    if (type == StationType::Invalid)     return QObject::tr("invalid");
    if (type == StationType::Unknown)     return QObject::tr("unknown");
	if (type == StationType::PDO16)       return QObject::tr("PDO-16");
    if (type == StationType::PDO16N)      return QObject::tr("PDO-16N");
    if (type == StationType::PDO32)       return QObject::tr("PDO-32");
    if (type == StationType::PDO32N)      return QObject::tr("PDO-32N");
	if (type == StationType::PDO48)       return QObject::tr("PDO-48");
    if (type == StationType::UPV2)        return QObject::tr("UPV-2");
    if (type == StationType::UPV4)        return QObject::tr("UPV-4");
    if (type == StationType::UPV6)        return QObject::tr("UPV-6");
    if (type == StationType::UPV2N)       return QObject::tr("UPV-2N");
    if (type == StationType::UPV4N)       return QObject::tr("UPV-4N");
    if (type == StationType::UPV_A)       return QObject::tr("UPV-A");
    if (type == StationType::SL1)         return QObject::tr("SL1");
    if (type == StationType::SL2)         return QObject::tr("SL2");
    if (type == StationType::UM_300_2)    return QObject::tr("UM-300-2");
    LOG_ERROR(0, "Unknown StationType of (%1)", convert::to_utype(type));
    return IPS_STR("Error");
}
inline bool fromString(QString const& str, StationType& val , QLocale const& = {}) noexcept {
    if (str == QObject::tr("invalid"))			val = StationType::Invalid;
    else if (str == QObject::tr("unknown"))		val = StationType::Unknown;
    else if (str == QObject::tr("PDO-16"))		val = StationType::PDO16;
    else if (str == QObject::tr("PDO-16N"))		val = StationType::PDO16N;
    else if (str == QObject::tr("PDO-32"))		val = StationType::PDO32;
    else if (str == QObject::tr("PDO-32N"))		val = StationType::PDO32N;
    else if (str == QObject::tr("PDO-48"))		val = StationType::PDO48;
    else if (str == QObject::tr("UPV-2"))		val = StationType::UPV2;
    else if (str == QObject::tr("UPV-4"))		val = StationType::UPV4;
    else if (str == QObject::tr("UPV-6"))		val = StationType::UPV6;
    else if (str == QObject::tr("UPV-2N"))		val = StationType::UPV2N;
    else if (str == QObject::tr("UPV-4N"))		val = StationType::UPV4N;
    else if (str == QObject::tr("UPV-A"))		val = StationType::UPV_A;
    else if (str == QObject::tr("SL1"))		    val = StationType::SL1;
    else if (str == QObject::tr("SL2"))			val = StationType::SL2;
    else if (str == QObject::tr("UM-300-2"))	val = StationType::UM_300_2;
    else {
         LOG_ERROR(0, "Invalid station::StationType value (%1)", str);
         val = StationType::Invalid;
         return false;
    }
    return true;
}

/* begin namespace station_type */
namespace station_type {
using items_t = std::array<StationType,14>;

static inline items_t defaultStationTypes() noexcept {
    return {            
		StationType::PDO16
		, StationType::PDO16N
		, StationType::PDO32
		, StationType::PDO32N
		, StationType::PDO48
		, StationType::UPV2
		, StationType::UPV4
		, StationType::UPV6
		, StationType::UPV2N
		, StationType::UPV4N
		, StationType::UPV_A
		, StationType::SL1
		, StationType::SL2
		, StationType::UM_300_2
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
