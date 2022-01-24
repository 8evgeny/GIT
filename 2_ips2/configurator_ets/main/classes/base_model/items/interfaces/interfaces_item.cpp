#include "interfaces_item.h"
#include "include/tr.h"
#include "classes/algo.h"
#include "classes/app.h"
#include "classes/json.h"
#include "classes/defines.h"
#include "ui_interfaces_item.h"
#include <QJsonObject>

InterfacesItem::InterfacesItem(BaseItem* item)
     : BaseItem(item)
{
}
InterfacesItem::~InterfacesItem()
{   
    deshow();
}
InterfacesItem* InterfacesItem::show(QWidget* content, QWidget* settings)
{
    ips::algo::removeChildren(content);
    ips::algo::removeChildren(settings);
    ips::algo::createContent(ui, content);    
    return this;
}
void InterfacesItem::deshow()
{
    delete ui;
    ui = nullptr;
}
bool InterfacesItem::error(BaseItem*) const
{
    return true;
}
QString InterfacesItem::name(BaseItem*) const
{
    return iInterfaces::tr("Interfaces");
}
QJsonValue InterfacesItem::serialize() const
{
    return QJsonObject {{IPS_STR("SipStations"), App::model()->sipStationsItem()->serialize()}};
}
bool InterfacesItem::clear()
{        
    return true;
}
bool InterfacesItem::deserialize(QJsonObject const&)
{    
    return true;
}
