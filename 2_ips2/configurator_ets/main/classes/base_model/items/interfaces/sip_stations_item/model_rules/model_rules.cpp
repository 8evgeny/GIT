#include <gt/json.h>
#include "classes/app.h"
#include "classes/json.h"
#include "classes/defines.h"
#include "include/convert/string.h"
#include "classes/base_model/items/interfaces/sip_stations_item/model_rules/model_rules.h"

ModelRules::ModelRules(QObject* parent)
    : inherited(parent)
{
}
int ModelRules::rowCount(const QModelIndex&) const
{
    return inherited::items().size();
}
int ModelRules::columnCount(const QModelIndex&) const
{
    return COUNT_COLUMNS_RULES;
}
Qt::ItemFlags ModelRules::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return index.column() == 1
        ? Qt::ItemIsSelectable | Qt::ItemIsEnabled
        : Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
bool ModelRules::insertRows(int position, int rows, const QModelIndex& index)
{
    if(rows < 1)
        return false;

    inherited::beginInsertRows(index, position, position + rows - 1);
    for (auto i = 0; i!= rows; ++i) {
         auto item = new Rule(IPS_STR("_X"), IPS_STR("PABX-1"));
         inherited::items().insert(position + rows - 1, *item);
    }
    inherited::endInsertRows();
    return true;
}
QVariant ModelRules::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto const& item = inherited::item(index.row());
        if (index.column() == 0)
            return convert::truncate(item.rule(), 64);
        else if (index.column() == 1)
             return item.destination();
    }

    return {};
}
QVariant ModelRules::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return iDestination::tr("Rules");
            case 1:
                return iDestination::tr("Directions");
        }
     }
    return QVariant();
}
bool ModelRules::setData(const QModelIndex& index, const QVariant& value, int role)
{    
    if (!index.isValid() || index.row() >= inherited::items().size() || index.row() < 0)
        return false;

    if (role == Qt::EditRole) {
        auto& item = inherited::item(index.row());
        if (index.column() == 0)
            item.setRule(value.toString());
        if (index.column() == 1)
            item.setDestination(value.toString());
    }
    return true;
}
QJsonArray ModelRules::serialize() const {
    QJsonArray res;
    std::transform(items().cbegin(), items().cend(), std::back_inserter(res), [](const auto& item) {
        return (QJsonObject{
            {IPS_STR("rule"), item.rule()}
           ,{IPS_STR("destination"), item.destination()}
    });
    });
    return res;
}
bool ModelRules::deserialize(QJsonArray const& arr)
{
    auto i = 0;
    auto pos = 0;
    for (auto const& a: arr) {
        if (a.isUndefined() || !a.isObject())
            return false;

        auto item = items().value(i);
        IPS_JSON_TO_STR(a.toObject(), IPS_STR("rule"), rule)
        item.setRule(rule);

        IPS_JSON_TO_STR(a.toObject(), IPS_STR("destination"), destination)
        item.setDestination(destination);
        insertItem(pos++, item);
   }
    return true;
}
