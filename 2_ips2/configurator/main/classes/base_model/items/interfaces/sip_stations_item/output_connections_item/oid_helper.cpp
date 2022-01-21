#include "classes/app.h"
#include "oid_helper.h"
#include "classes/base_model/items/global_parameters_item/global_parameters.h"

#define MAX_VALUE_Oid 9999

OidHelper::value_t OidHelper::next() noexcept
{
    auto items = App::model()->oids();
    if(items.isEmpty())
       return App::model()->globalParametersItem()->callerid();

    auto it = items.back();
    for (value_t i = 1; i != 100; ++i) {
        if(it > MAX_VALUE_Oid)
           it = 1;
        if(!items.contains(it))
           return it;

        ++it;
    }
    return App::model()->globalParametersItem()->callerid();
}
bool OidHelper::contains(value_t val) noexcept
{
    auto const& items = App::model()->oids();
    return items.contains(val);
}
