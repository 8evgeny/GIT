#pragma once
#include "include/log.h"
#include "include/convert/enum.h"

enum class ProtocolType: std::int8_t {
      Invalid             = 0
    , Unknown             = 1
    , UDP                 = 2
    , TCP                 = 3
};

inline QString toString(ProtocolType const& protocolType = ProtocolType::UDP) noexcept {
    if (protocolType == ProtocolType::Invalid)             return IPS_STR("Invalid");
    if (protocolType == ProtocolType::Unknown)             return IPS_STR("Unknown");
    if (protocolType == ProtocolType::UDP)                 return IPS_STR("UDP");
    if (protocolType == ProtocolType::TCP)                 return IPS_STR("TCP");
    LOG_ERROR(0, "Unknown ProtocolType of (%1)", convert::to_utype(protocolType));
    return IPS_STR("Error");
}
/* begin namespace protocol_type */
namespace protocol_type {
inline ProtocolType fromString(QString const& str, QLocale const& = {}) noexcept {
    if (str == IPS_STR("Invalid"))                        return ProtocolType::Invalid;
    else if (str == IPS_STR("Unknown"))                   return ProtocolType::Unknown;
    else if (str == IPS_STR("UDP"))                   return ProtocolType::UDP;
    else if (str == IPS_STR("TCP"))                   return ProtocolType::TCP;
    return {};
}
using items_t = std::array<ProtocolType, 2>;
inline items_t defaultProtocolTypes() noexcept {
    return {
            ProtocolType::UDP
           ,ProtocolType::TCP
    };
}
static inline void addItem(QComboBox* box, ProtocolType type) noexcept {
    box->addItem(toString(type), convert::to_utype(type));
}
    inline void fill(QComboBox* box) noexcept {
        for(auto c: defaultProtocolTypes())
            addItem(box, c);
    }
}
// end namespace protocol_type
Q_DECLARE_METATYPE(ProtocolType)
