#pragma once
#include <QList>
#include "main/classes/oid.h"

class OidHelper final {
 public:
    using class_name = OidHelper;
    using items_t    = oid_list_t;
    using value_t    = typename items_t::value_type;

 public:
    static value_t next() noexcept;
    static bool contains(value_t val) noexcept;
};
