#pragma once
#include <QList>
#include "global_parameters.h"

class App;

class IpHelper final {
 public:
    using class_name = IpHelper;
    using items_t    = QList<QString>;
    using value_t    = typename items_t::value_type;

 public:
    static value_t next() noexcept;
    static bool contains(value_t const& val);
    static bool contains(QVariant const& val);
};
