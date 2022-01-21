#include "classes/app.h"
#include "key_helper.h"

KeyHelper::value_t KeyHelper::next(StationItem* item) noexcept
{
    auto const& items = App::model()->keys(item);
    if(items.isEmpty())
       return 1;

    auto it = items.back();
    for(value_t i = 1; i != 100; ++i) {      
        if(it > 999)
            it = 1;
        if(!items.contains(it))
            return it;
        ++it;
    }
    return 1;
}
bool KeyHelper::contains(value_t val, StationItem* item) noexcept
{
    auto const& items = App::model()->keys(item);
    return items.contains(val);
}
