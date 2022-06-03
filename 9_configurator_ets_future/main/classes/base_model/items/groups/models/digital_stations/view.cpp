#include "view.h"
#include "include/log.h"
#include "classes/app.h"
#include "include/convert/string.h"
#include "../include/validators/string_validator.h"
#include "classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"

ModelTableView::ModelTableView(QObject *parent)
    : inherited(parent)
{
}
int ModelTableView::columnCount(const QModelIndex&) const
{
    return 2;
}
QVariant ModelTableView::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return QVariant();

    auto contact = inherited::items()[index.row()];
    if (role == Qt::DisplayRole || role == updateRole) {
        if (index.column() == 0)
             return contact->cid();
        if (index.column() == 1)
             return contact->stationName();
    }

    return QVariant();
}
QVariant ModelTableView::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:                
                return iDesc::tr("CID");
            case 1:
                return iDesc::tr("Name");
        }
     }
    return QVariant();
}
