#pragma once
#include <QString>
#include "classes/app.h"
#include "include/typedefs.h"
#include "include/convert/integer.h"
#include "include/convert/string.h"
#include "classes/options/language.h"
#include "include/convert/enum.h"
#include "main/classes/base_types/duration_types.h"

#define IPS_ONCE 0
using duration_t = DurationType;

inline QString durationToString(duration_t const& duration) noexcept {
    if(duration == duration_t::ONCE)
        return IPS_STR("ONCE");

    return duration_type::toString(duration);
}
inline duration_t durationFromString(QString const& duration) noexcept {
    if(duration == IPS_STR("ONCE"))
        return duration_t::ONCE;

    return duration_type::fromString(duration);
}
