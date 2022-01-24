#include <QPainter>
#include "include/tr.h"
#include "model_programmer.h"
#include "include/validators/ip_validator.h"
#include "classes/base_model/items/global_parameters_item/ip_helper.h"

ModelProgrammer::ModelProgrammer(QObject* parent)
    : inherited(parent)
{    
}
int ModelProgrammer::rowCount(const QModelIndex&) const
{
    return inherited::items().size();
}
int ModelProgrammer::columnCount(const QModelIndex&) const
{
    return 7;
}
Qt::ItemFlags ModelProgrammer::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return ((index.column() == 1) || (index.column() == 4))
        ? Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable
        : Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QVariant ModelProgrammer::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return {};

    auto const& item = inherited::items().value(index.row());
    LOG_FATAL(item, "Cannot get StationItem");
    if (role == Qt::DisplayRole) {        
        if (index.column() == 1)
           return item->stationItem()->name();
        else if (index.column() == 2)
           return item->serialNumber();
        else if (index.column() == 3)
           return item->mac();
        else if (index.column() == 4)
           return item->stationItem()->ip();
        else if (index.column() == 5)
           return role == Qt::DisplayRole
             ? toString(item->stationItem()->type())
             : QVariant::fromValue<StationType>(item->stationItem()->type());
    }
    if (role == StatusRole && index.column() == IP_STATUS_POSITION)
        return item->status();

    if (role == Qt::DecorationRole && index.column() == IP_STATUS_POSITION) {
        auto const color = item->status()
                ? QIcon(QStringLiteral(":/svg/green_icon.svg"))
                : QIcon(QStringLiteral(":/svg/red_icon.svg"));
        return color;
    }    
    return {};
}
bool ModelProgrammer::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
        return false;

    auto const& item = inherited::items().value(index.row());
    LOG_FATAL(item, "Cannot get StationItem");

    if(role == StatusRole && index.column() == IP_STATUS_POSITION)
       item->setStatus(value.toBool());           

    if(index.column() == 1 && role == Qt::EditRole) {
        if (!value.toString().isEmpty()) {
            item->stationItem()->setStationName(convert::truncate(value.toString(), 64));
        }
    }
    if(index.column() == IP_POSITION && role == Qt::EditRole) {
        if (IpValidator::isValid(value) && !IpHelper::contains(value)) {
            item->stationItem()->setIp(value.toString());
        }
    }
    return true;
}
QVariant ModelProgrammer::headerData(int section, Qt::Orientation orientation, int role) const
{
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return iInfo::tr("Status IP");
                case 1:
                    return iInfo::tr("Name");
                case 2:
                    return iInfo::tr("SerialNumber");
                case 3:
                    return GT_STR("MAC");
                case 4:
                    return GT_STR("IP");
                case 5:
                    return iInfo::tr("Type");
                case 6:
                    return iInfo::tr("Write");
            }
         }

    return {};
}
