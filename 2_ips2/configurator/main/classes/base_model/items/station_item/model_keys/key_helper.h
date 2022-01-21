#pragma once
#include "classes/app.h"

class App;
class StationItem;

class KeyHelper final {
 public:
    using class_name = KeyHelper;
    using value_t    = uint;

 public:
    static value_t next(StationItem* item) noexcept;
    static bool contains(value_t val, StationItem* item) noexcept;
};
