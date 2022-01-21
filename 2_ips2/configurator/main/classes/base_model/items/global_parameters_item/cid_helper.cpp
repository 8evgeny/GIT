#include "classes/app.h"
#include "cid_helper.h"

#define MAX_VALUE_CID 9999

QList<uint> CidHelper::getCids() noexcept
{
    auto const& itemsCids = App::model()->globalParametersItem()->cids();
    QList<uint> cids;
    for (auto itemCid : itemsCids) {
        auto cid = itemCid->cid();
        cids.append(cid);
    }
    return cids;
}
CidHelper::value_t CidHelper::next() noexcept
{
    auto items = getCids();
    if(items.isEmpty())
       return App::model()->globalParametersItem()->callerid();

    auto it = items.back();
    for (value_t i = 1; i != 100; ++i) {
        if(it > MAX_VALUE_CID)
           it = 1;
        if(!items.contains(it))
           return it;

        ++it;
    }
    return App::model()->globalParametersItem()->callerid();
}
bool CidHelper::contains(value_t val) noexcept
{
    auto const& items = getCids();
    return items.contains(val);
}
