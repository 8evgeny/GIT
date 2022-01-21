#include "classes/app.h"
#include "gid_helper.h"

GidHelper::value_t GidHelper::next() noexcept
{
    auto const& items = App::model()->gids();
    if(items.isEmpty())
       return 1;

    auto it = items.back();
    for (value_t i = 1; i != 100; ++i) {
        if(it > 9999)
           it = 1;
        if(!items.contains(it))
            return it;
        ++it;
    }
    return 1;
}
bool GidHelper::contains(value_t val) noexcept
{
    auto const& items = App::model()->gids();    
    return items.contains(val);
}
