#pragma once
#include <QList>
#include "classes/gid.h"

class GidHelper final {
 public:
    using class_name = GidHelper;
    using items_t    = gid_list_t;
    using value_t    = typename items_t::value_type;

 public:
    static value_t next() noexcept;
    static bool contains(value_t val) noexcept;
};
