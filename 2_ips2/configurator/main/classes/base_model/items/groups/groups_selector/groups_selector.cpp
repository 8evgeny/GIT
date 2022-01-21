#include "groups_selector.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/groups/groups_item.h"

GroupsSelector::GroupsSelector(QObject* parent)
    : inherited(parent)    
{
}
GroupsSelector::~GroupsSelector()
{
}
QList<GroupsSelector::station_t*>const& GroupsSelector::stations(const QModelIndex &index) const noexcept
{
    return m_model.stationList(index);
}
void GroupsSelector::update(const QModelIndex &index, QList<StationItem*> const& stations)
{
    m_model.setListStation(index, stations);
}
bool GroupsSelector::clear() noexcept
{
    ips::algo::clear<ModelGroups>(&m_model);
    return true;
}
QJsonArray GroupsSelector::serialize() const
{    
    return m_model.serialize();
}
bool GroupsSelector::deserialize(QJsonArray const& arr)
{
    return m_model.deserialize(arr);
}
