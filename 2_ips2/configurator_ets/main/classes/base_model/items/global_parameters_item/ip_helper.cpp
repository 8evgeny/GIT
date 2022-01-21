#include <QString>
#include "ip_helper.h"
#include "classes/app.h"
#include "include/convert/string.h"

IpHelper::value_t IpHelper::next() noexcept
{
    auto const& items = App::model()->ips();
    if(items.isEmpty())
        return App::model()->globalParametersItem()->ip();

    for(auto i = 1u; i != 256; ++i) {
        auto const& item = items.back();
        if(auto indx = item.lastIndexOf(QLatin1String(".")); indx != -1) {
            auto id = convert::toUint(item.mid(indx+1, item.size() - 1)) + i;
            if(id > 256)
                id = 0;
            auto nextip = IPS_STR("%1.%2").arg(item.mid(0, indx), convert::toString(id));
            if(!items.contains(nextip))
                return nextip;
        }
    }
    return App::model()->globalParametersItem()->ip();
}
bool IpHelper::contains(value_t const& val)
{
    auto const& items = App::model()->ips();
    return items.contains(val);
}
bool IpHelper::contains(QVariant const& val)
{
    return contains(val.toString());
}
