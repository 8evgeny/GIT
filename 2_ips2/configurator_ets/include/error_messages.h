#pragma once
#include <QString>
#include "typedefs.h"

/* begin namespace error_messages */
namespace error_messages {

static inline QString create(QString const& msg, int type, QString const desc) noexcept {
    return IPS_STR("%1 (%2) | %3").arg(msg, QString::number(type), desc);
}
}
/* end namespace error_messages */
