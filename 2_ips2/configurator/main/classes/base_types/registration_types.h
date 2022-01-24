#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class RegistrationType: std::uint32_t {    
      Invalid                = 0
    , Unknown                = 1
    , ON                     = 2
    , OFF                    = 3
};

inline QString toString(RegistrationType const& registrationType = RegistrationType::Unknown) noexcept {
    if (registrationType == RegistrationType::Invalid)                return IPS_STR("Invalid");
    if (registrationType == RegistrationType::Unknown)                return IPS_STR("Unknown");
    if (registrationType == RegistrationType::ON)                     return IPS_STR("ON");
    if (registrationType == RegistrationType::OFF)                    return IPS_STR("OFF");
    LOG_ERROR(0, "Unknown ProtocolType of (%1)", convert::to_utype(registrationType));
    return IPS_STR("Error");
}
/* begin namespace registration_type */
namespace registration_type {
inline RegistrationType fromString(QString const& str, QLocale const& = {}) noexcept {
    if (str == iFunction::tr("Invalid"))                       return RegistrationType::Invalid;
    else if (str == iFunction::tr("Unknown"))                  return RegistrationType::Unknown;
    else if (str == iFunction::tr("ON"))                       return RegistrationType::ON;
    else if (str == iFunction::tr("OFF"))                      return RegistrationType::OFF;
    return {};
}
using items_t = std::array<RegistrationType, 2>;
inline items_t defaultRegistrationTypes() noexcept {
    return {            
            RegistrationType::ON
           ,RegistrationType::OFF
    };
}
static inline void addItem(QComboBox* box, RegistrationType type) noexcept {
    box->addItem(toString(type), convert::to_utype(type));
}
    inline void fill(QComboBox* box) noexcept {
        for(auto c: defaultRegistrationTypes())
            addItem(box, c);
    }
}
// end namespace registration_type
Q_DECLARE_METATYPE(RegistrationType)
