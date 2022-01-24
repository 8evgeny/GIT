#pragma once
#include <QObject>
#include "include/log.h"
#include "include/typedefs.h"
#include "main/classes/cid.h"
#include "main/classes/base_model/items/station_item/station_item.h"
#include "classes/base_model/items/groups/models/calls/groups_calls.h"

class GroupsSelector final : public QObject
{
    Q_OBJECT
    IPS_DISABLE_COPY(GroupsSelector)

    using inherited  = QObject;
    using class_name = GroupsSelector;
    using station_t  = StationItem;

public:
     explicit GroupsSelector(QObject* parent = nullptr);
    ~GroupsSelector();

public slots:    
    void update(const QModelIndex &index, QList<StationItem*> const& stations);

public:    
    QList<station_t*> const& stations(const QModelIndex &index) const noexcept;
    bool clear() noexcept;
    QJsonArray serialize() const;
    bool deserialize(QJsonArray const& arr);

public:
    ModelGroups* modelGroups() noexcept {
        return &m_model;
    }   

private:   
   ModelGroups m_model;   
};
