#pragma once
#include <QList>
#include "global_parameters.h"

class CidHelper final {
 public:
    using class_name = CidHelper;
    using items_t    = cid_list_t;
    using value_t    = typename items_t::value_type;

 //private:
 public:
    static QList<uint> getCids() noexcept;

 public:
    static value_t next() noexcept;
    static bool contains(value_t val) noexcept;
};
